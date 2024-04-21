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

public:
	static void LoadTexture(string fileName);
	static void LoadTexture(string fileName, sf::IntRect bounds);
	static void LoadTexture(string mapName, string fileName, sf::IntRect bounds);
	static sf::Texture &GetTexture(string textureName);
	static void Clear(); // call this once at the end of main
};