#include "Networks.h"

// TODO(you): World state replication lab session

#include "ReplicationManagerClient.h"

void ReplicationManagerClient::Read(const InputMemoryStream& packet)
{
	while (packet.RemainingByteCount() > sizeof(uint32))
	{
		uint32 networkId;
		packet >> networkId;
		ReplicationAction action;
		packet >> action;

		switch (action)
		{
		case ReplicationAction::Create:
		{
			GameObject* newGO = Instantiate();

			if (newGO)
			{
				packet >> newGO->position.x;
				packet >> newGO->position.y;

				packet >> newGO->angle;

				packet >> newGO->sprite->order;

				packet >> newGO->sprite->pivot.x;
				packet >> newGO->sprite->pivot.y;

				packet >> newGO->size.x;
				packet >> newGO->size.y;

				packet >> newGO->sprite->color.r;
				packet >> newGO->sprite->color.g;
				packet >> newGO->sprite->color.b;
				packet >> newGO->sprite->color.a;



				packet >> newGO->collider->type;

				switch (newGO->collider->type)
				{
				case ColliderType::Player:
				{
					newGO->collider = App->modCollision->addCollider(ColliderType::Player, newGO);
					break;
				}
				case ColliderType::Laser:
				{
					newGO->collider = App->modCollision->addCollider(ColliderType::Laser, newGO);
					break;
				}
				default:
					break;
				}


				packet >> newGO->tag;

				App->modLinkingContext->registerNetworkGameObjectWithNetworkId(newGO, networkId);
			}
			break;
		}

		case ReplicationAction::Destroy:
		{
			GameObject* gameObject = App->modLinkingContext->getNetworkGameObject(networkId);

			if (gameObject)
			{
				App->modLinkingContext->unregisterNetworkGameObject(gameObject);
				Destroy(gameObject);
			}

			break;
		}

		case ReplicationAction::None:
		{
			break;
		}

		case ReplicationAction::Update:
		{
			GameObject* gameObject = App->modLinkingContext->getNetworkGameObject(networkId);

			if (gameObject)
			{
				packet >> gameObject->position.x;
				packet >> gameObject->position.y;

				packet >> gameObject->angle;
			}
			break;
		}
		}
	}
}
