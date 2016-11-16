// fix map iter
// fix copy constructor
// fix isnull
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <string>


class ParameterServer
{
public:
	template <class T>
	bool param(std::string path, T & v, T def);

	template <class T>
	bool param(std::string path, T & v);

	YAML::Node top;

	bool append(std::string path);

	YAML::Node solve(std::string path);

	YAML::Node solveparent(std::string path);

};

YAML::Node ParameterServer::solveparent(std::string path)
{
	std::stringstream ss(path);
	std::string item;
	YAML::Node c = top;
	YAML::Node parent = top;
	while (std::getline(ss, item, '/')) 
	{
		if(c.isMap())
		{
			parent = c;
			c = c[item];
		}
		else
		{
			return YAML::Node(); // fail
		}
  	}
  	return parent;
}

YAML::Node ParameterServer::solve(std::string path)
{
	std::stringstream ss(path);
	std::string item;
	YAML::Node c = top;
	while (std::getline(ss, item, '/')) 
	{
		if(c.isMap())
		{
			c = c[item];
		}
		else
		{
			return YAML::Node(); // fail
		}
  	}
  	return c;
}

template <class T>
bool ParameterServer::param(std::string path, T & v, T def)
{
	YAML::Node s = solve(path);
	if(!s.IsNull())
	{
		v = def;
	}
	else
	{
		v = s.as<T>();
	}
	return true;
}


template <class T>
bool ParameterServer::param(std::string path, T & v)
{
	YAML::Node s = solve(path);
	if(!s.isNull())
	{
		return false;
	}
	else
	{
		v = s.as<T>();
		return true;
	}
}


bool ParameterServer::append(std::string path)
{
	YAML::Node q = YAML::LoadFile(path);
	if(!q.isMap())
		return false;
	// q has to be a dictionary
	if(top.isNull())
	{
		top = q;
	}
	else
	{
		for(auto & it: q)
		{
			top[it.first] = it.second;
		}
	}
	return true;
}

int main(int argc, char const *argv[])
{
	ParameterServer ps;
	ps.append("config.yaml");
	ps.append("other.yaml");

	int x = 0;
	ps.param("ciao",x,10);

	std::string q;
	ps.param("name",q,std::string("ciao"));

	return 0;
}