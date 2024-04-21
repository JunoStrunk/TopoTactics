#include "TextureManager.h"

// redeclare the variable
unordered_map<string, sf::Texture> TextureManager::textures;

void TextureManager::LoadTexture(string fileName)
{
	string path = "../assets/";
	path += fileName + ".png";

	textures[fileName].loadFromFile(path);
}

void TextureManager::LoadTexture(string fileName, sf::IntRect bounds)
{
	string path = "../assets/";
	path += fileName + ".png";

	textures[fileName].loadFromFile(path, bounds);
}

void TextureManager::LoadTexture(string mapName, string fileName, sf::IntRect bounds)
{
	string path = "../assets/";
	path += fileName + ".png";

	textures[mapName].loadFromFile(path, bounds);
}

sf::Texture &TextureManager::GetTexture(string textureName)
{
	// if texture DOESN"T exist...
	if (textures.find(textureName) == textures.end()) // didn't find it
		LoadTexture(textureName);
	// load it first? then return it?
	return textures[textureName];
}

void TextureManager::Clear()
{
	textures.clear(); // get rid of stored objects
}