#pragma once

#include "CoreMinimal.h"
#include "UDIContext.generated.h"

class UWorld;
class AActor;

UCLASS()
class UNREALDI_API UUDIContext : public UObject
{
	GENERATED_BODY()

public:
    template <class T>
	T* Construct()
    {
        return Construct<T>(T::StaticClass());
    }
    
    template <class T>
    T* Construct(TSubclassOf<T> Class)
    {
        UObject* Object = NewObject<UObject>(this, Class);
        InjectServices(Object);
        return Cast<T>(Object);
    }

    template <class T>
    T* Spawn(UWorld* World)
    {
        return Spawn<T>(World, T::StaticClass());
    }

    template <class T>
    T* Spawn(UWorld* World, TSubclassOf<T> Class)
    {
        AActor* Actor = World->SpawnActor(Class);
        InjectServices(Actor);
        return Cast<T>(Actor);
    }

    template <class T>
    T* GetService()
    {
        return Cast<T>(GetService(T::StaticClass()));
    }

    UObject* GetService(UClass* Class);
    void RegisterService(UObject* Service);
    void InjectServices(UObject* Object);

private:
    UPROPERTY()
    TMap<FString, UObject*> Services;

    void RegisterService(UObject* Service, UClass* ServiceClass);
};
