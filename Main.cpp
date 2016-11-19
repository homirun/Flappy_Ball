#include <Siv3D.hpp>
#include<vector>

using namespace std;
void Main()
{
	Window::Resize(800, 500);
	Window::SetTitle(L"Flappy Ball");
	int first = 0, flag = 0, frame = 0;
	static const Vec2 g(0, 0.5);	//重力加速度
	static const Font font(23);
	static const Font scorefont(15);
	static const Point wallSize(Window::Width(), Window::Height());	//壁の初期座標
	static const String HiScore_BSC_Path = L"hiscore_BSC.txt";
	static const String HiScore_ADV_Path = L"hiscore_ADV.txt";
	vector<RectF> wall;
	vector<RectF> checkhit;
	vector<int> hitflag;
	int score = 0;
	int HiScore_BSC;
	int HiScore_ADV;
	TextWriter writer;
	TextReader reader;
	Vec2 PP = { 0, 0 };	//プレイヤーの初期座標
	Circle P(100, 0, 10);
	Rect Difficult_L(Window::Width() / 2-225, Window::Height()/ 2+140 , 200, 100);
	Rect Difficult_R(Window::Width() / 2+25, Window::Height() / 2 + 140, 200, 100);
	Color Menu_R_Color = Color(111,111,111);
	Color Menu_L_Color = Color(100, 200, 75);
	bool HiScore_Flag = false;
	int Difficult_Flag = 0;

	reader.open(HiScore_BSC_Path);
	HiScore_BSC = Parse<int>(reader.readAll());
	reader.close();
	reader.open(HiScore_ADV_Path);
	HiScore_ADV = Parse<int>(reader.readAll());
	reader.close();
	/****デバッグモード切り替え(default:false)***/
	static const bool Debug_Mode=false; 
	/********************************************/

	while (System::Update())
	{
		if (first == 0) {	//最初の１回だけ行う処理
			font(L"Flappy Ball").drawCenter({ Window::Width() / 2, Window::Height() / 2 });
			scorefont(L"Enterでゲームスタート").drawCenter({ Window::Width() / 2, Window::Height() / 2 + 60 });
			
			if (Difficult_R.leftClicked==true) {
				Menu_R_Color = Color(255, 158, 1);
				Menu_L_Color = Color(111,111,111);
				Difficult_Flag = 1;
			}
			if (Difficult_L.leftClicked == true) {
				Menu_R_Color = Color(111,111,111);
				Menu_L_Color = Color(100, 200, 75);
				Difficult_Flag = 0;
			}
			if (Difficult_Flag == 0) {
				scorefont(L"HiScore(BSC): ", (int)HiScore_BSC).drawCenter({ Window::Width() / 2,Window::Height() / 2 + 90 });
			}
			else if (Difficult_Flag == 1) {
				scorefont(L"HiScore(ADV): ", (int)HiScore_ADV).drawCenter({ Window::Width() / 2,Window::Height() / 2 + 90 });
			}
			Difficult_L.draw(Menu_L_Color);
			Difficult_R.draw(Menu_R_Color);
			scorefont(L"Basic").draw(Window::Width() / 2 - 210, Window::Height() / 2 + 140);
			scorefont(L"Advance").draw(Window::Width() / 2 + 40, Window::Height() / 2 + 140);
			if (Debug_Mode == true) {
				scorefont(L"Debug_Mode...").drawCenter({ Window::Width() / 2,Window::Height() / 2 + 120 });


			}
			if (Input::KeyEnter.clicked) {
				first = 1;
			}
		}

		else if (flag == 0 &&Difficult_Flag==0) {	//Basic

			frame++;
			PP += g;
			scorefont(L"Difficult: Basic").draw(5, 5);
			scorefont(score).draw(5, 30);

			if (Input::KeyEnter.pressed) {
				PP = { 0, -7.0 };
			}
			P.moveBy(PP);
			if (P.y > Window::Height()) {	//落下した時
				flag = 1;
			}
			if (P.y <= 10) {	//上に飛び出そうな時
				P.y = 10;
			}
			if (frame % 120 == 0) {	//壁出現タイミング
				int rand = Random(100, 350);
				wall.push_back(RectF(wallSize.x, 0, 30, rand));
				wall.push_back(RectF(wallSize.x, rand + 100, 30, Window::Height() - 100 - rand));
				checkhit.push_back(RectF(wallSize.x, rand, 30, 100));
				hitflag.push_back(0);
			}
			for (unsigned int i = 0; i < wall.size(); i++) {
				wall[i].draw();
				wall[i].moveBy(Vec2{ -3, 0 });

				if (P.intersects(wall[i])&&Debug_Mode==false) {	//当たり判定
					flag = 1;
				}
				//if (wall[i].x < -30) {
					//wall.erase(wall.begin() + i);
				//}
			}
			for (unsigned int i = 0; i < checkhit.size(); i++) {	//当たり判定の四角形
				checkhit[i].moveBy(Vec2{ -3, 0 });
				if (P.intersects(checkhit[i]) && hitflag[i] == 0) {
					score++;
					hitflag[i] = 1;
				}
				//if (checkhit[i].x < 0) {
					//checkhit.erase(checkhit.begin() + i);
					//hitflag.erase(hitflag.begin() + i);
				//}
			}
			if (frame >= 12000) {	//オーバーフロー防止
				frame = 0;
			}
			P.draw(Color(0, 200, 255));
		}
		else if (flag == 0 && Difficult_Flag == 1){	//Advance
			frame++;
			PP += g;
			scorefont(L"Difficult: Advance").draw(5, 5);
			scorefont(score).draw(5, 30);

			if (Input::KeyEnter.pressed) {
				PP = { 0, -10.0 };
			}
			P.moveBy(PP);
			if (P.y > Window::Height()) {	//落下した時
				flag = 1;
			}
			if (P.y <= 10) {	//上に飛び出そうな時
				P.y = 10;
			}
			if (frame % 100 == 0) {	//壁出現タイミング
				int rand = Random(100, 350);
				wall.push_back(RectF(wallSize.x, 0, 30, rand));
				wall.push_back(RectF(wallSize.x, rand + 100, 30, Window::Height() - 100 - rand));
				checkhit.push_back(RectF(wallSize.x, rand, 30, 100));
				hitflag.push_back(0);
			}
			for (unsigned int i = 0; i < wall.size(); i++) {
				wall[i].draw();
				wall[i].moveBy(Vec2{ -3, 0 });

				if (P.intersects(wall[i]) && Debug_Mode == false) {	//当たり判定
					flag = 1;
				}
				//if (wall[i].x < -30) {
				//wall.erase(wall.begin() + i);
				//}
			}
			for (unsigned int i = 0; i < checkhit.size(); i++) {	//当たり判定の四角形
				checkhit[i].moveBy(Vec2{ -3, 0 });
				if (P.intersects(checkhit[i]) && hitflag[i] == 0) {
					score++;
					hitflag[i] = 1;
				}
				//if (checkhit[i].x < 0) {
				//checkhit.erase(checkhit.begin() + i);
				//hitflag.erase(hitflag.begin() + i);
				//}
			}
			if (frame >= 12000) {	//オーバーフロー防止
				frame = 0;
			}
			P.draw(Color(0, 200, 255));
		}
		else if (flag == 1) {	//死んだ時
			font(L"GameOver").drawCenter({ Window::Width() / 2, Window::Height() / 2 });
			font(L"score:", score).drawCenter({ Window::Width() / 2, Window::Height() / 2 + 50 });
			if (Difficult_Flag == 0) {
				if (score > HiScore_BSC) {
					HiScore_Flag = true;
					font(L"ハイスコア更新").drawCenter({ Window::Width() / 2,Window::Height() / 2 + 80 });
				}
			}
			else if (Difficult_Flag == 1) {
				if (score > HiScore_ADV) {
					HiScore_Flag = true;
					font(L"ハイスコア更新").drawCenter({ Window::Width() / 2,Window::Height() / 2 + 80 });
				}
			}
			if (HiScore_Flag == true && Difficult_Flag == 0) {
				writer.open(HiScore_BSC_Path, TextEncoding::UTF8);
				//writer.clear();
				writer.write(Format(score));
				writer.close();
				HiScore_BSC = score;
				HiScore_Flag = false;
			}
			else if (HiScore_Flag == true && Difficult_Flag == 1) {
				writer.open(HiScore_ADV_Path, TextEncoding::UTF8);
				writer.write(Format(score));
				writer.close();
				HiScore_ADV = score;
				HiScore_Flag = false;
			}
			if (Input::KeyEnter.clicked) {	//リスタート準備
				wall.clear();
				checkhit.clear();
				hitflag.clear();
				frame = 0;
				score = 0;
				flag = 0;
				wall.clear();
				hitflag.clear();
				checkhit.clear();
				P.y = 0;
			}
			continue;
		}
	}
}

