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
        return Cast<T>(Construct(T::StaticClass()));
    }
    
    template <class T>
    T* Spawn(UWorld* World)
    {
        return Cast<T>(Spawn(World, T::StaticClass()));
    }

    template <class T>
    T* GetService()
    {
        return Cast<T>(GetService(T::StaticClass()));
    }

    UObject* Construct(UClass* Class);
    AActor* Spawn(UWorld* World, UClass* Class);
    UObject* GetService(UClass* Class);
    void RegisterService(UObject* Service);
    void InjectServices(UObject* Object);

private:
    UPROPERTY()
    TMap<FString, UObject*> Services;
};
