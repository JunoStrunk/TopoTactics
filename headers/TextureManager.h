#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
using std::string;
using std::unordered_map;

/* Provided by Professor Joshua Fox */

class TextureManager
{
	// static == once and only one of these in memory ever
	static unordered_map<string, sf::Texture> textures;
	static void LoadTexture(string fileName);

public:
	static sf::Texture &GetTexture(string textureName);
	static void Clear(); // call this once at the end of main
};