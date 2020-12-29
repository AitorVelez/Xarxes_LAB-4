#pragma once

// TODO(you): World state replication lab session
#include <map>
#include <list>

enum class ReplicationAction
{
	None, Create, Update, Destroy
};

struct ReplicationCommand
{
	//ReplicationCommand(ReplicationAction action, uint32 id) : action(action), networkId(id) {};
	ReplicationAction action;
	uint32 networkId;
};

class ReplicationManagerServer
{
public:

	void Create(uint32 networkId);
	void Update(uint32 networkId);
	void Destroy(uint32 networkId);

	void Write(OutputMemoryStream& packet);

	std::map<uint32, ReplicationAction> commands_map;

public:
	//std::list<ReplicationCommand*> commands;


};