#include "Networks.h"

// TODO(you): World state replication lab session

#include "ReplicationManagerServer.h"
using namespace std;

void ReplicationManagerServer::Create(uint32 networkId)
{
	//commands.push_back(new ReplicationCommand(ReplicationAction::Create, networkId));
	commands_map[networkId] = ReplicationAction::Create;
}

void ReplicationManagerServer::Update(uint32 networkId)
{
	//commands.push_back(new ReplicationCommand(ReplicationAction::Update, networkId));
	commands_map[networkId] = ReplicationAction::Update;
}

void ReplicationManagerServer::Destroy(uint32 networkId)
{
	//commands.push_back(new ReplicationCommand(ReplicationAction::Destroy, networkId));
	commands_map[networkId] = ReplicationAction::Destroy;
}

void ReplicationManagerServer::Write(OutputMemoryStream& packet)
{

	if (commands_map.size() == 0)
		return;
	for (std::map<uint32, ReplicationAction>::iterator it = commands_map.begin(); it != commands_map.end(); ++it)
	{
		packet << (*it).first;
		packet << (*it).second;

		if ((*it).second == ReplicationAction::Create)
		{
			GameObject* newGO = App->modLinkingContext->getNetworkGameObject((*it).first, false);
			if (!newGO)
			{
				newGO = Instantiate();
				newGO->networkId = (*it).first;
				packet << 1;
			}
			else
			{
				packet << 0;
			}
			packet << newGO->position.x;
			packet << newGO->position.y;
			packet << newGO->size.x;
			packet << newGO->size.y;
			packet << newGO->angle;

			std::string texture = "";
			if (newGO->sprite)
			{
				texture = newGO->sprite->texture->filename;
				packet << texture;
				packet << newGO->sprite->order;
			}
			else
			{
				packet << texture;
				packet << 0;
			}
			if (newGO->behaviour)
				packet << newGO->behaviour->type();
			else
				packet << BehaviourType::None;

			packet << newGO->tag;
		}

		else if ((*it).second == ReplicationAction::Update)
		{
			GameObject* newGO = App->modLinkingContext->getNetworkGameObject((*it).first);
			packet << newGO->position.x;
			packet << newGO->position.y;
			packet << newGO->size.x;
			packet << newGO->size.y;
			packet << newGO->angle;
			if (newGO->behaviour)
				newGO->behaviour->write(packet);
		}

		ReplicationCommand commands_map;
		commands_map.action = (*it).second;
		commands_map.networkId = (*it).first;
	}
	commands_map.clear();

}



