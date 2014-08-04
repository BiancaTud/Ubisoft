#include "Collision.h"
#include "Sprite.h"
#include "SpriteManager.h"

Collision::Collision(){
	hitPlayer = false;
	hitEnemy = false;
}

void Collision::Init(CSprite* sprite1, CSprite* sprite2){
	p1.x = p4.x = sprite1->model_mat[3][0];
	p1.y = p2.y = sprite1->model_mat[3][1];
	if (sprite1->type == player){
		p2.x = p3.x = p1.x + 0.4f;
		p3.y = p4.y = p1.y + 1.0f;
	}
	else if(sprite1->type==player_proj){
		p2.x = p3.x = p1.x + 0.4f/3;
		p3.y = p4.y = p1.y + 1.0f/3;

	}


	q1.x = q4.x = sprite2->model_mat[3][0];
	q1.y = q2.y = sprite2->model_mat[3][1];

	if (sprite2->type == enemy){
		q2.x = q3.x = q1.x + 0.4f;
		q3.y = q4.y = q1.y + 0.9f;
	}

	else if (sprite2->type == enemy_proj){

		q2.x = q3.x = q1.x + 0.1f;
		q3.y = q4.y = q1.y + 0.1f;

	}
}

bool Collision::CheckCollision(CSprite* sprite1, CSprite* sprite2){

	Init(sprite1, sprite2);
	bool hit = false;
	if ((p1.x > q1.x && p1.x < q2.x && p1.y < q4.y && p1.y > q1.y) ||
		(p2.x > q1.x && p2.x < q2.x && p2.y < q4.y && p2.y > q1.y) ||
		(p3.x > q1.x && p3.x < q2.x && p3.y < q4.y && p3.y > q1.y) ||
		(p4.x > q1.x && p4.x < q2.x && p4.y < q4.y && p4.y > q1.y) ||

		(q1.x > p1.x && q1.x < p2.x && q1.y < p4.y && q1.y > p1.y) ||
		(q2.x > p1.x && q2.x < p2.x && q2.y < p4.y && q2.y > p1.y) ||
		(q3.x > p1.x && q3.x < p2.x && q3.y < p4.y && q3.y > p1.y) ||
		(q4.x > p1.x && q4.x < p2.x && q4.y < p4.y && q4.y > p1.y)){
		hit = true;


	}

	return hit;
}


void Collision::ResolveCollisionPlayer(CSprite *sprite1, CSprite *sprite2){


    //coliziune intre player si un inamic
		bool check = CheckCollision(sprite1, sprite2);
		hitPlayer = false;
		if (check == true){
			sprite1->SetPosition(glm::vec3(sprite1->GetPosition().x,
				- 4.0f, sprite1->GetPosition().z));
		//	check = false;
			sprite1->life--;
			hitPlayer = true;
			

		}



}

void Collision::ResolveCollisionEnemy(vector<CSprite*> v1, vector <CSprite*> v2){

	bool check = false;
	hitEnemy = false;
	int index = -1;
	int pivot = 0;
	for (int j = 0; j < v2.size(); j++){
		for (int i = 0; i < v1.size(); i++){

			check = CheckCollision(v1[i], v2[j]);
			if (check == true){
				v2[j]->life--;
				if (v2[j]->life == 0){

					v2[j]->SetPosition(glm::vec3(-4.0f - j, v2[j]->getFirstParabolaY(-3.0f - j), v2[j]->GetPosition().z));
				}
				hitEnemy = true;
				index = i;
				CSpriteManager::Get()->RemoveSprite(CSpriteManager::Get()->projectiles[i]->index-pivot);
				CSpriteManager::Get()->projectiles.erase(CSpriteManager::Get()->projectiles.begin() + (i-pivot));
				v1.erase(v1.begin() + i-pivot);
				cout << "KILLED" << endl;
				check = false;
				pivot++;
			}
		}

	}



}

bool Collision::getStatusPlayer(){

	return hitPlayer;
}

bool Collision::getStatusEnemy(){

	return hitEnemy;
}


Collision::~Collision()
{

}