#include "common.h"
#include "Utils.h"
#include "SpriteManager.h"
#include "Sprite.h"

#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



// Pool de spriteuri
static CSprite					g_AllSprites[MAX_SCENE_SPRITE_COUNT];
static unsigned int				g_AllSpritesCount			= 0;
// Toate sprite-urile din scena
static CSprite					*g_AllSpritesInScene[MAX_SCENE_SPRITE_COUNT];
static unsigned int				g_AllSpritesInSceneCount	= 0;


//vector cu toate Sprite-urile din scena
vector<CSprite*> all;


CSpriteManager					CSpriteManager::g_me;

// Cautare liniara dupa nume in vectorul m_AllLoadedAnimations
SAnimation*		CSpriteManager::GetAnimation(const char *name)
{
	for(unsigned int i = 0; i< m_NumAllLoadedAnimations; i++)
	{
		SAnimation *  ptrTexture = &m_AllLoadedAnimations[i];
		if(strcmp(ptrTexture->m_Name, name) == 0)
		{
			return ptrTexture;
		}
	}
	return NULL;
}

// Cautare liniara dupa nume in vectorul m_AllLoadedSubTextures
SDictEntry* CSpriteManager::FindInSubTextureVector( const char * name)
{
	for(unsigned int i = 0; i< m_NumLoadedSubTextures; i++)
	{
		SDictEntry *  ptrTexture = &m_AllLoadedSubTextures[i];
		if(strcmp(ptrTexture->m_Name, name) == 0)
		{
			return ptrTexture;
		}
	}
	return NULL;
}

// Cautare liniara dupa nume in vectorul m_AllLoadedTextures
SLoadedTexture* CSpriteManager::FindInTextureVector( const char * name)
{
	for(unsigned int i = 0; i< m_NumLoadedTextures; i++)
	{
		SLoadedTexture *  ptrTexture = &m_AllLoadedTextures[i];
		if(strcmp(ptrTexture->m_Name, name) == 0)
		{
			return ptrTexture;
		}
	}
	return NULL;
}

// incarcam din fisierul animations.xml toate datele despre animatii
void CSpriteManager::LoadAllAnimations()
{
	char dataPath[] = "../data/SpriteSheets/";
	std::string location(dataPath);
	location.append("animations.xml");
		
	rapidxml::file<> xmlFile(location.c_str()); 
	rapidxml::xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	rapidxml::xml_node<> *pRoot = doc.first_node();
	
	for (rapidxml::xml_node<> *pNode = pRoot->first_node("Animation"); pNode; pNode = pNode->next_sibling())
	{
		SAnimation * new_entry = &m_AllLoadedAnimations[m_NumAllLoadedAnimations++];
		
		rapidxml::xml_attribute<> *anim_begin	= pNode->first_attribute("begin");
		assert(anim_begin);
		SDictEntry * beginframe = FindInSubTextureVector(anim_begin->value());
		new_entry->m_BeginFrame = beginframe - m_AllLoadedSubTextures;
		
		rapidxml::xml_attribute<> *anim_end		= pNode->first_attribute("end");
		assert(anim_end);
		SDictEntry * endframe = FindInSubTextureVector(anim_end->value());
		new_entry->m_EndFrame = endframe - m_AllLoadedSubTextures ;
		rapidxml::xml_attribute<> *anim_speed	= pNode->first_attribute("speed");
		assert(anim_speed);
		new_entry->m_Speed = (float)atof(anim_speed->value());
		
		
		rapidxml::xml_attribute<> *anim_name	= pNode->first_attribute("name");
		strcpy(new_entry->m_Name, anim_name->value());

	}
}

// incarcam din fisierul all_sprite_sheets.xml toate sprite sheet-urile ce urmeaza sa le folosim in joc
void CSpriteManager::LoadAllSprites()
{
	char dataPath[] = "../data/SpriteSheets/";
	std::vector<std::string> all_sprites_names;
	{
		std::string location(dataPath);
		location.append("all_sprite_sheets.xml");
		
		rapidxml::file<> xmlFile(location.c_str()); 
		rapidxml::xml_document<> doc;
		doc.parse<0>(xmlFile.data());
		rapidxml::xml_node<> *pRoot = doc.first_node();
		
		for (rapidxml::xml_node<> *pNode = pRoot->first_node("SpriteSheet"); pNode; pNode = pNode->next_sibling())
		{
			all_sprites_names.push_back(pNode->first_attribute("file")->value());
		}

	}
	
	for (unsigned int i = 0; i< all_sprites_names.size(); i++)
	{
		// Formam path-ul catre .xml-ul  cu datele despre textura mare
		std::string locationSpr(dataPath);
		locationSpr.append(all_sprites_names[i]);

		// Deschidem fisierul xml si citim din el datele
		rapidxml::file<> xmlFile(locationSpr.c_str()); 
		rapidxml::xml_document<> doc;
		doc.parse<0>(xmlFile.data());

		rapidxml::xml_node<>*		pRoot	= doc.first_node();
		rapidxml::xml_attribute<>*	img		= pRoot->first_attribute("imagePath");
		assert(img);
		
		char *imageName = img->value();
		// Vedem daca avem imaginea incarcata
		// Populam vectorul de texturi "MARI"
		SLoadedTexture*  foundLoadedTexture = FindInTextureVector(imageName);
		if (foundLoadedTexture == NULL) //nu am gasit imaginea si trebuie sa o incarcam
		{
			std::string locationTex(dataPath);
			locationTex.append(imageName);
			int width, height;
			int texture = LoadTexture(locationTex.c_str(),width,height);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			foundLoadedTexture				= &m_AllLoadedTextures[m_NumLoadedTextures++];
			foundLoadedTexture->m_Id		= texture;
			foundLoadedTexture->m_Height	= height;
			foundLoadedTexture->m_Width		= width;

			strcpy(foundLoadedTexture->m_Name, imageName);
			
		}
	
		// Populam vectorul cu subtexturi
		for (rapidxml::xml_node<> *pNode = pRoot->first_node("SubTexture"); pNode; pNode = pNode->next_sibling())
		{
			// citim coordonatele in pixeli si le transformam in [0 ,1]
			SDictEntry * new_entry = &m_AllLoadedSubTextures[m_NumLoadedSubTextures++];
			
			
			rapidxml::xml_attribute<> *spr_x	= pNode->first_attribute("x");
			assert(spr_x);
			new_entry->m_TextureOffsets.x		= (float)atof(spr_x->value())/(float)foundLoadedTexture->m_Width;

			rapidxml::xml_attribute<> *spr_y	= pNode->first_attribute("y");
			assert(spr_y);
			new_entry->m_TextureOffsets.y		= (float)atof(spr_y->value()) / (float)foundLoadedTexture->m_Height;
			
			rapidxml::xml_attribute<> *spr_w	= pNode->first_attribute("width");
			assert(spr_w);
			new_entry->m_TextureOffsets.z		= (float)atof(spr_w->value()) / (float)foundLoadedTexture->m_Width;

			rapidxml::xml_attribute<> *spr_h	= pNode->first_attribute("height");
			assert(spr_h);
			new_entry->m_TextureOffsets.w		= (float)atof(spr_h->value())/(float)foundLoadedTexture->m_Height;
			
			rapidxml::xml_attribute<> *spr_name		= pNode->first_attribute("name");
			strcpy(new_entry->m_Name, spr_name->value());

			new_entry->m_RefTexture = foundLoadedTexture;

		}
		doc.clear();
	}

}


CSpriteManager::CSpriteManager()
{
	m_NumLoadedTextures = 0;
	m_NumLoadedSubTextures = 0;
	m_NumAllLoadedAnimations = 0;
	check = false;
}

void CSpriteManager::Init()
{
	// Incarcam texturile mari si subtexturile  
	LoadAllSprites();
	// Incarcam animatiile
	LoadAllAnimations();

	//Incarcam shaderul
	const char* fragment_shader = LoadFileInMemory("../data/fragmentShader.glsl");
	const char* vertex_shader	= LoadFileInMemory("../data/vertexShader.glsl");

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	m_GlobalShaderProgram = glCreateProgram();
	glAttachShader(m_GlobalShaderProgram, fs);
	glAttachShader(m_GlobalShaderProgram, vs);
	glLinkProgram(m_GlobalShaderProgram);

	delete[] fragment_shader;
	delete[] vertex_shader;
	// end
	// luam locatia din shader la care vom pune textura 
	int tex_loc = glGetUniformLocation(m_GlobalShaderProgram, "basic_texture");
	glUseProgram(m_GlobalShaderProgram);
	glUniform1i(tex_loc, 0); // use active texture 0

	// generam matricea de proiectie ortografica
	m_ProjectionMatrix		= glm::ortho(SCREEN_LEFT, SCREEN_RIGHT, SCREEN_BOTTOM, SCREEN_TOP, 0.1f, 100.0f);
	// luam locatia matricei de proiectie din shader
	m_ProjectionMatrixLoc	= glGetUniformLocation(m_GlobalShaderProgram, "u_proj_matrix");
		

}

CSprite * CSpriteManager::AddSprite(const char * name, int type)
{
	// luam un sprite nou din vector-ul Pool
	CSprite*		newSprite = &g_AllSprites[g_AllSpritesCount++];


	//setam tipul
	newSprite->SetType(type);

	// luam subtextura dupa nume
	SDictEntry*		SpriteSubTexture	= FindInSubTextureVector(name);
	assert(SpriteSubTexture != NULL);
	
	// Formam coordonatele uv
	int idx = 0;
	float uv[8];
	
	glm::vec4 &TexOff = SpriteSubTexture->m_TextureOffsets;

	uv[0] = TexOff.x;					uv[1] =  1.0f-(TexOff.y + TexOff.w);
	uv[2] = TexOff.x + TexOff.z;		uv[3] =  1.0f-(TexOff.y + TexOff.w) ;
	uv[4] = TexOff.x + TexOff.z;		uv[5] =  1.0f-TexOff.y;
	uv[6] = TexOff.x;					uv[7] =  1.0f-TexOff.y;
	
	// calculam aspect ratio-ul
	float aspectRatio = TexOff.z/ TexOff.w;
	
	// initializam sprite-ul
	newSprite->Init(m_GlobalShaderProgram, uv, aspectRatio);
	newSprite->SetTexture(SpriteSubTexture->m_RefTexture->m_Id);
	
	// adaugam spriteul in scena

	all.push_back(newSprite);
	return newSprite;
}

void CSpriteManager::RemoveSprite(int i)
{	

	// cand scoatem un sprite, mutam ultimul sprite peste el si scadem dimensiunea sprite-urilor active
	if (i != 0 && (i<=all.size()-1)){
		all[i] = all[all.size() - 1];
		all.erase(all.end() - 1);
	}

}

// functie de comparatie dupa adancime folosita la sortare (qsort)
int SpriteDepthCompare (const void * a, const void * b)
{
	CSprite *_a = *((CSprite**)a);
	CSprite *_b = *((CSprite**)b);

  if ( _a->GetPosition().z < _b->GetPosition().z ) 
	  return -1;
  if ( _a->GetPosition().z == _b->GetPosition().z) 
	  return 0;
  return 1;
}



// Desenarea sprite-urilor
void CSpriteManager::Draw()
{
	//qsort(g_AllSpritesInScene, g_AllSpritesInSceneCount, sizeof(g_AllSpritesInScene[0]), SpriteDepthCompare);
	
	for (unsigned int i = 0; i< all.size(); i++)
	{
		
		glUniformMatrix4fv(m_ProjectionMatrixLoc, 1, GL_FALSE, glm::value_ptr(m_ProjectionMatrix));
		
		if (all[i]->life>0){
			all[i]->Draw();
		}

		all[i]->index = i;
	}
}

// Update-ul animatiilor sprite-urilor
void CSpriteManager::Update(float dt)
{
		
	for (unsigned int i = 0; i< all.size(); i++)
	{
		if (all[i]->life>0)
			all[i]->UpdateAnimation(dt);
	}

	glm::vec3 posProj;
	//limitare proiectile
	int pivot = 0;
	for (int i = 0; i < projectiles.size(); i++){
		//daca depasesc ecranul sunt sterse
		if (projectiles[i]->GetPosition().y > SCREEN_TOP - 0.4f)
		{
			RemoveSprite(CSpriteManager::Get()->projectiles[i]->index-pivot);
			projectiles.erase(CSpriteManager::Get()->projectiles.begin() + i-pivot);
			pivot++;


		}
		//altfel este updatata pozitia lor
		else{
			posProj = projectiles[i]->GetPosition();
			projectiles[i]->SetPosition(glm::vec3(posProj.x, posProj.y +projectiles[i]->speed * dt, posProj.z));
				
		}
	}


}

CSpriteManager::~CSpriteManager()
{
}