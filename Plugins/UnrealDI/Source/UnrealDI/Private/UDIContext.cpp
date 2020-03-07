#include "UDIContext.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "UObject/Class.h"

#include "UDILog.h"

UObject* UUDIContext::Construct(UClass* Class)
{
    UObject* Object = NewObject<UObject>(this, Class);
    InjectServices(Object);
    return Object;
}

AActor* UUDIContext::Spawn(UWorld* World, UClass* Class)
{
    AActor* Actor = World->SpawnActor(Class);
    InjectServices(Actor);
    return Actor;
}

UObject* UUDIContext::GetService(UClass* Class)
{
    UObject** Service = Services.Find(Class->GetName());
    return Service != nullptr ? *Service : nullptr;
}

void UUDIContext::RegisterService(UObject* Service)
{
    Services.Add(Service->GetClass()->GetName(), Service);

    UE_LOG(LogUDI, Log, TEXT("Registering %s as service of type %s."), *Service->GetName(), *Service->GetClass()->GetName());
}

void UUDIContext::InjectServices(UObject* Object)
{
    UClass* Class = Object->GetClass();

    UE_LOG(LogUDI, Log, TEXT("Constructing a new %s..."), *Class->GetName());

    for (TFieldIterator<UProperty> PropIt(Class); PropIt; ++PropIt)
    {
        UProperty* Property = *PropIt;

        if (Property->HasMetaData(TEXT("Inject")))
        {
            UE_LOG(LogUDI, Log, TEXT("Property %s is injected."), *Property->GetName());

            UObjectProperty* ObjectProperty = Cast<UObjectProperty>(Property);

            if (ObjectProperty != nullptr)
            {
                UObject* Service = GetService(ObjectProperty->PropertyClass);

                if (Service != nullptr)
                {
                    UE_LOG(LogUDI, Log, TEXT("Injecting property %s with %s."), *Property->GetName(), *Service->GetName());

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
            UE_LOG(LogUDI, Verbose, TEXT("%s is not injected."), *Property->GetName());
        }
    }

    if (Class->HasMetaData(TEXT("InjectedService")))
    {
        RegisterService(Object);
    }
}
