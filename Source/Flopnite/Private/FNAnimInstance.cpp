//

#include "FNAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FNCharacter.h"

UFNAnimInstance::UFNAnimInstance()
{
    /* */
    Speed = 0.0f;
    IsInAir = false;
    Direction = 0.0f;
    Yaw = 0.0f;
    Pitch = 0.0f;
    JumpEnabled = false;
}

void UFNAnimInstance::NativeUpdateAnimation( float DeltaSeconds )
{
    Super::NativeUpdateAnimation( DeltaSeconds );

    AFNCharacter * OwningChar = Cast< AFNCharacter >( TryGetPawnOwner() );
    
    if ( OwningChar != nullptr )
    {
        
        JumpEnabled = OwningChar->JumpEnabled;
        if (JumpEnabled)
        {
            UE_LOG(LogTemp, Warning, TEXT("jump enabled is true"));
            OwningChar->JumpEnabled = false;
        }
        Speed = OwningChar->GetVelocity().Size();
        Direction = CalculateDirection( OwningChar->GetVelocity(),  OwningChar->GetActorRotation() );
        IsInAir = OwningChar->GetCharacterMovement()->IsFalling();
        
        const FRotator& Delta = OwningChar->GetControlRotation() - OwningChar->GetActorRotation();
        const FRotator& AimRotation = FRotator(Pitch, Yaw, 0.0f);

        const FRotator& ResultRotation = FMath::RInterpTo(AimRotation, Delta, DeltaSeconds, 15.0f);

        Pitch = FMath::ClampAngle(ResultRotation.Pitch, -90, 90);
        Yaw = FMath::ClampAngle(ResultRotation.Yaw, -90, 90);
    }
}

void UFNAnimInstance::NOTIFY_Jump()
{
    AFNCharacter * OwningChar = Cast< AFNCharacter >( TryGetPawnOwner() );

    /*if (EnableJump)
    {
        OwningChar->Jump();        
    }*/
    
}

void UFNAnimInstance::NOTIFY_JogStart()
{
    AFNCharacter * OwningChar = Cast< AFNCharacter >( TryGetPawnOwner() );

    //EnableJump = false;

    //SomeThing here
    
}