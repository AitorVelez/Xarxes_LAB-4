#pragma once

// TODO(you): World state replication lab session
#include <string>
#include <iostream>

class ReplicationManagerClient
{
public:
	void Read(const InputMemoryStream& packet);

	//Files
	std::string spaceship1 = "spacecraft1.png";
	std::string spaceship2 = "spacecraft2.png";
	std::string spaceship3 = "spacecraft3.png";
	std::string laser = "laser.png";
	std::string explosion = "explosion1.png";
};