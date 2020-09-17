#include "UDIContext.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "UObject/Class.h"

#include "UDILog.h"

UObject* UUDIContext::GetService(UClass* Class)
{
    UObject** Service = Services.Find(Class->GetName());
    return Service != nullptr ? *Service : nullptr;
}

void UUDIContext::RegisterService(UObject* Service)
{
    RegisterService(Service, Service->GetClass());
}

void UUDIContext::RegisterService(UObject* Service, UClass* ServiceClass)
{
    Services.Add(ServiceClass->GetName(), Service);

    UE_LOG(LogUDI, Log, TEXT("Registering %s as service of type %s."), *Service->GetName(), *ServiceClass->GetName());
}

void UUDIContext::InjectServices(UObject* Object)
{
    UClass* Class = Object->GetClass();

    UE_LOG(LogUDI, Log, TEXT("Constructing a new %s..."), *Class->GetName());

    for (TFieldIterator<FProperty> PropIt(Class); PropIt; ++PropIt)
    {
        FProperty* Property = *PropIt;

        if (Property->HasMetaData(TEXT("Inject")))
        {
            UE_LOG(LogUDI, Verbose, TEXT("Property %s is injected."), *Property->GetName());

            FObjectProperty* ObjectProperty = CastFieldChecked<FObjectProperty>(Property);

            if (ObjectProperty != nullptr)
            {
                UObject* Service = GetService(ObjectProperty->PropertyClass);

                if (Service != nullptr)
                {
                    UE_LOG(LogUDI, Log, TEXT("Injecting %s into property %s."), *Service->GetName(), *Property->GetName());

                    UObject* Address = ObjectProperty->ContainerPtrToValuePtr<UObject>(Object);
                    ObjectProperty->SetObjectPropertyValue(Address, Service);
                }
                else
                {
                    UE_LOG(LogUDI, Warning, TEXT("Property %s can't be injected, as no service with matching type has been constructed before."), *Property->GetName());
                }
            }
            else
            {
                UE_LOG(LogUDI, Warning, TEXT("Property %s is not a property of type UObject."), *Property->GetName());
            }
        }
        else
        {
            UE_LOG(LogUDI, Verbose, TEXT("Property %s is not injected."), *Property->GetName());
        }
    }

    while (Class)
    {
        if (Class->HasMetaData(TEXT("Inject")))
        {
            RegisterService(Object, Class);
            return;
        }
        
        Class = Class->GetSuperClass();
    }
}
