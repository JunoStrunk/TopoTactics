#include "TextureManager.h"

// redeclare the variable
unordered_map<string, sf::Texture> TextureManager::textures;

void TextureManager::LoadTexture(string fileName)
{
	string path = "../../images/";
	path += fileName + ".png";

	textures[fileName].loadFromFile(path);
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