#include "Networks.h"

// TODO(you): World state replication lab session

#include "ReplicationManagerClient.h"

void ReplicationManagerClient::Read(const InputMemoryStream& packet)
{
	while (packet.RemainingByteCount() > 0)
	{
		uint32 networkId;
		packet >> networkId;
		ReplicationAction action = ReplicationAction::None;
		packet >> action;

		if (action == ReplicationAction::Create)
		{
			int tested = 0;
			packet >> tested;
			GameObject* newGO = App->modLinkingContext->getNetworkGameObject(networkId);

			bool test = false;
			if (newGO)
				test = true;

			newGO = Instantiate();

			if (test == false)
			{
				GameObject* newGO_destroy = App->modLinkingContext->getNetworkGameObject(networkId, false);
				if (newGO_destroy)
				{
					App->modLinkingContext->unregisterNetworkGameObject(newGO_destroy);
					Destroy(newGO_destroy);
				}

				App->modLinkingContext->registerNetworkGameObjectWithNetworkId(newGO, networkId);
			}

			packet >> newGO->position.x;
			packet >> newGO->position.y;
			packet >> newGO->size.x;
			packet >> newGO->size.y;
			packet >> newGO->angle;

			std::string texture;
			packet >> texture;
			newGO->sprite = App->modRender->addSprite(newGO);
			packet >> newGO->sprite->order;
			if (newGO->sprite)
			{
				if (texture == spaceship1)
					newGO->sprite->texture = App->modResources->spacecraft1;
				else if (texture == spaceship2)
					newGO->sprite->texture = App->modResources->spacecraft2;
				else if (texture == spaceship3)
					newGO->sprite->texture = App->modResources->spacecraft3;
				else if (texture == laser)
					newGO->sprite->texture = App->modResources->laser;
				else if (texture == explosion)
				{
					newGO->sprite->texture = App->modResources->explosion1;
					newGO->animation = App->modRender->addAnimation(newGO);
					newGO->animation->clip = App->modResources->explosionClip;
					App->modSound->playAudioClip(App->modResources->audioClipExplosion);
				}
			}

			BehaviourType type;
			packet >> type;
			if (type == BehaviourType::Spaceship)
				newGO->behaviour = App->modBehaviour->addSpaceship(newGO);
			else if (type == BehaviourType::Laser)
				newGO->behaviour = App->modBehaviour->addLaser(newGO);
			packet >> newGO->tag;

			if (test)
			{
				Destroy(newGO);
			}
			if (tested == 1)
			{
				App->modLinkingContext->unregisterNetworkGameObject(newGO);
				Destroy(newGO);
			}
		}

		else if (action == ReplicationAction::Update)
		{
			GameObject* newGO = App->modLinkingContext->getNetworkGameObject(networkId);
			packet >> newGO->position.x;
			packet >> newGO->position.y;
			packet >> newGO->size.x;
			packet >> newGO->size.y;
			packet >> newGO->angle;
			if (newGO->behaviour)
				newGO->behaviour->read(packet);
		}

		else if (action == ReplicationAction::Destroy)
		{
			GameObject* newGO = App->modLinkingContext->getNetworkGameObject(networkId);
			if (newGO)
			{
				App->modLinkingContext->unregisterNetworkGameObject(newGO);
				Destroy(newGO);
			}
		}
	}
}
