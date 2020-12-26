#include "Networks.h"

// TODO(you): World state replication lab session

#include "ReplicationManagerServer.h"
using namespace std;

void ReplicationManagerServer::Create(uint32 networkId)
{
	commands.push_back(new ReplicationCommand(ReplicationAction::Create, networkId));
}

void ReplicationManagerServer::Update(uint32 networkId)
{
	commands.push_back(new ReplicationCommand(ReplicationAction::Update, networkId));
}

void ReplicationManagerServer::Destroy(uint32 networkId)
{
	commands.push_back(new ReplicationCommand(ReplicationAction::Destroy, networkId));
}

void ReplicationManagerServer::Write(OutputMemoryStream& packet)
{

	for (list<ReplicationCommand*>::iterator it = commands.begin(); it != commands.end(); ++it)
	{
		GameObject* go = App->modLinkingContext->getNetworkGameObject((*it)->networkId);

		switch ((*it)->action)
		{
		case ReplicationAction::None:
			break;

		case ReplicationAction::Create:
			if (go)
			{
				packet << (*it)->networkId;
				packet << (*it)->action;

				packet << go->position.x;
				packet << go->position.y;

				packet << go->angle;

				packet << go->sprite->order;

				packet << go->sprite->pivot.x;
				packet << go->sprite->pivot.y;

				packet << go->size.x;
				packet << go->size.y;

				packet << go->sprite->color.r;
				packet << go->sprite->color.g;
				packet << go->sprite->color.b;
				packet << go->sprite->color.a;



				packet << go->colliderType;

				packet << go->sprite->textureType;

				packet << go->tag;


			}
			break;

		case ReplicationAction::Update:
			if (go)
			{
				packet << (*it)->networkId;
				packet << (*it)->action;

				packet << go->position.x;
				packet << go->position.y;
				packet << go->angle;
			}
			break;

		case ReplicationAction::Destroy:

			packet << (*it)->networkId;
			packet << (*it)->action;

			break;

		default:
			break;
		}

		delete* it;
		*it = nullptr;
	}
	commands.clear();

}



