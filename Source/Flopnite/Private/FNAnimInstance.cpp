//

#include "FNAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FNCharacter.h"

UFNAnimInstance::UFNAnimInstance()
{
    /* */
    IsInAir = false;
    Direction = 0.0;
}

void UFNAnimInstance::NativeUpdateAnimation( float DeltaSeconds )
{
    Super::NativeUpdateAnimation( DeltaSeconds );

    if ( TryGetPawnOwner() != nullptr )
    {
        Speed = TryGetPawnOwner()->GetVelocity().Size();
        Direction = CalculateDirection( TryGetPawnOwner()->GetVelocity(),  TryGetPawnOwner()->GetActorRotation() );

        AFNCharacter * OwningChar = Cast< AFNCharacter >( TryGetPawnOwner() );
        if (OwningChar != nullptr)
        {
            IsInAir = OwningChar->GetCharacterMovement()->IsFalling();
        }
        
    }
    
}

void UFNAnimInstance::NOTIFY_Jump()
{
    AFNCharacter * OwningChar = Cast< AFNCharacter >( TryGetPawnOwner() );

    if (EnableJump)
    {
        OwningChar->Jump();        
    }
    
}

void UFNAnimInstance::NOTIFY_JogStart()
{
    AFNCharacter * OwningChar = Cast< AFNCharacter >( TryGetPawnOwner() );

    EnableJump = false;

    //SomeThing here
    
}