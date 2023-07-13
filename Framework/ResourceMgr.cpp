#include "stdafx.h"
#include "ResourceMgr.h"
#include "rapidcsv.h"

ResourceMgr::~ResourceMgr()
{
	for (auto pair : mapTexture)
	{
		//delete pair.second;
		get<0>(pair.second);
	}
	mapTexture.clear();
	for (auto pair : mapFont)
	{
		get<0>(pair.second);
		//delete pair.second;
	}
	mapFont.clear();
	for (auto pair : mapSoundBuffer)
	{
		get<0>(pair.second);
		//delete pair.second;
	}
	mapSoundBuffer.clear();
	for (auto pair : mapAnimationClip)
	{
		get<0>(pair.second);
		//delete pair.second;
	}
	mapAnimationClip.clear();
}

void ResourceMgr::Init()
{
	//LoadFromCSV("tables/defalutResource.csv", true);
}

void ResourceMgr::UnLoadAll()
//순회하면서 tuple의 second 값이 false인 경우만 지우기
{
	{
		auto it = mapTexture.begin();
		while (it != mapTexture.end())
		{
			if (!get<1>(it->second)) //false인 경우에는 지우기
			{
				delete get<0>(it->second);
				it = mapTexture.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	{
		auto it = mapSoundBuffer.begin();
		while (it != mapSoundBuffer.end())
		{
			if (!get<1>(it->second)) //false인 경우에는 지우기
			{
				delete get<0>(it->second);
				it = mapSoundBuffer.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	{
		auto it = mapFont.begin();
		while (it != mapFont.end())
		{
			if (!get<1>(it->second)) //false인 경우에는 지우기
			{
				delete get<0>(it->second);
				it = mapFont.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	{
		auto it = mapAnimationClip.begin();
		while (it != mapAnimationClip.end())
		{
			if (!get<1>(it->second)) //false인 경우에는 지우기
			{
				delete get<0>(it->second);
				it = mapAnimationClip.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	//for (auto pair : mapTexture)
	//{
	//	//delete pair.second;
	//	get<0>(pair.second);
	//}
	//mapTexture.clear();
	//for (auto pair : mapFont)
	//{
	//	get<0>(pair.second);
	//	//delete pair.second;
	//}
	//mapFont.clear();
	//for (auto pair : mapSoundBuffer)
	//{
	//	get<0>(pair.second);
	//	//delete pair.second;
	//}
	//mapSoundBuffer.clear();
	//for (auto pair : mapAnimationClip)
	//{
	//	get<0>(pair.second);
	//	//delete pair.second;
	//}
	//mapAnimationClip.clear();
}

void ResourceMgr::LoadFromCSV(const string path, bool isDefault)
{
	rapidcsv::Document doc(path);
	vector<int>types = doc.GetColumn<int>(0);
	vector<string> paths = doc.GetColumn<string>(1);
	for (int i = 0; i < types.size(); i++)
	{
		Load((ResourceTypes)types[i], paths[i], isDefault);
	}
}

void ResourceMgr::Load(ResourceTypes t, const std::string path, bool isDefault)
{
	switch (t)
	{
	case ResourceTypes::Texture:
	{
		auto it = mapTexture.find(path);
		if (mapTexture.end() == it)
		{
			auto texture = new sf::Texture();
			texture->loadFromFile(path);
			mapTexture.insert({ path, {texture, isDefault} });
		}
	}
	break;
	case ResourceTypes::Font:
	{
		auto it = mapFont.find(path);
		if (mapFont.end() == it)
		{
			auto font = new sf::Font();
			font->loadFromFile(path);
			mapFont.insert({ path, { font, isDefault } });
		}
	}
	break;
	case ResourceTypes::SoundBuffer:
	{
		auto it = mapSoundBuffer.find(path);
		if (mapSoundBuffer.end() == it)
		{
			auto sb = new sf::SoundBuffer();
			sb->loadFromFile(path);
			mapSoundBuffer.insert({ path, { sb, isDefault } });
		}
	}
	break;
	case ResourceTypes::AnimationClip:
	{
		auto it = mapAnimationClip.find(path);
		if (mapAnimationClip.end() == it)
		{
			auto clip = new AnimationClip();
			clip->LoadFromFile(path);
			mapAnimationClip.insert({ path, { clip, isDefault} });
		}
	}
	break;
	}


}

//void ResourceMgr::Load(const std::vector<std::tuple<ResourceTypes, std::string>>& array)
//{
//	for (const auto& tuple : array)
//	{
//		Load(std::get<0>(tuple), std::get<1>(tuple));
//	}
//}

void ResourceMgr::Unload(ResourceTypes t, const std::string id)
{
	switch (t)
	{
	case ResourceTypes::Texture:
	{
		auto it = mapTexture.find(id);
		if (it != mapTexture.end())
		{
			delete get<0>(it->second);
			mapTexture.erase(it);
		}
	}
	break;
	case ResourceTypes::Font:
	{
		auto it = mapFont.find(id);
		if (it != mapFont.end())
		{
			if (!get<0>(it->second))
			{
				delete get<0>(it->second);
				mapFont.erase(it);
			}
			else
			{
				cout << "ERR: Default Resource" << endl;
			}
		}
	}
	break;
	case ResourceTypes::SoundBuffer:
	{
		auto it = mapSoundBuffer.find(id);
		if (it != mapSoundBuffer.end())
		{
			if (!get<0>(it->second))
			{
				delete get<0>(it->second);
				mapSoundBuffer.erase(it);
			}
			else
			{
				cout << "ERR: Default Resource" << endl;
			}
			
		}
	}
	break;
	case ResourceTypes::AnimationClip:
	{
		auto it = mapAnimationClip.find(id);
		if (it != mapAnimationClip.end())
		{
			if (!get<0>(it->second))
			{
				delete get<0>(it->second);
				mapAnimationClip.erase(it);
			}
			else
			{
				cout << "ERR: Default Resource" << endl;
			}
		}
	}
	}
}

//void ResourceMgr::Unload(const std::vector<std::tuple<ResourceTypes, std::string>>& array)
//{
//	for (const auto& tuple : array)
//	{
//		Unload(std::get<0>(tuple), std::get<1>(tuple));
//	}
//}

sf::Texture* ResourceMgr::GetTexture(const std::string& id)
{
	auto it = mapTexture.find(id);
	if (it != mapTexture.end())
	{
		return get<0>(it->second);
	}
	return nullptr;
}

sf::Font* ResourceMgr::GetFont(const std::string& id)
{
	auto it = mapFont.find(id);
	if (it != mapFont.end())
	{
		return get<0>(it->second);
	}
	return nullptr;
}

sf::SoundBuffer* ResourceMgr::GetSoundBuffer(const std::string& id)
{
	auto it = mapSoundBuffer.find(id);
	if (it != mapSoundBuffer.end())
	{
		return get<0>(it->second);
	}
	return nullptr;
}

AnimationClip* ResourceMgr::GetAnimationClip(const string& id)
{
	auto it = mapAnimationClip.find(id);
	if (it != mapAnimationClip.end())
	{
		return get<0>(it->second);
	}
	return nullptr;
}
