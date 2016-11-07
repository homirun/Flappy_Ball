#include <Siv3D.hpp>
#include<vector>

using namespace std;
void Main()
{
	Window::Resize(800, 500);
	Window::SetTitle(L"Flappy Ball");
	int fast = 0, flag = 0, frame = 0;
	static const Vec2 g(0, 0.5);	//重力加速度
	static const Font font(23);
	static const Font scorefont(15);
	static const Point wallSize(Window::Width(), Window::Height());	//壁の初期座標
	static const String HiScore_Path = L"hiscore.txt";
	vector<RectF> wall;
	vector<RectF> checkhit;
	vector<int> hitflag;
	int score = 0;
	int HiScore;
	TextWriter writer;
	TextReader reader;
	Vec2 PP = { 0, 0 };	//プレイヤーの初期座標
	Circle P(100, 0, 10);
	bool HiScore_Flag = false;

	reader.open(HiScore_Path);
	HiScore = Parse<int>(reader.readAll());
	reader.close();

	/****デバッグモード切り替え(default:false)***/
	static const bool Debug_Mode=false; 
	/********************************************/

	while (System::Update())
	{
		if (fast == 0) {	//最初の１回だけ行う処理
			font(L"Flappy Ball").drawCenter({ Window::Width() / 2, Window::Height() / 2 });
			scorefont(L"Enterでゲームスタート").drawCenter({ Window::Width() / 2, Window::Height() / 2 + 60 });
			scorefont(L"HiScore", (int)HiScore).drawCenter({Window::Width()/2,Window::Height()/2+90});
			if (Debug_Mode == true) {
				scorefont(L"Debug_Mode...").drawCenter({ Window::Width() / 2,Window::Height() / 2 + 120 });


			}
			if (Input::KeyEnter.clicked) {
				fast = 1;
			}
		}

		else if (flag == 0) {

			frame++;
			PP += g;

			scorefont(score).draw(5, 5);

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

		else if (flag == 1) {	//死んだ時
			font(L"GameOver").drawCenter({ Window::Width() / 2, Window::Height() / 2 });
			font(L"score:", score).drawCenter({ Window::Width() / 2, Window::Height() / 2 + 50 });
			if (score > HiScore) {
				HiScore_Flag = true;
				font(L"ハイスコア更新").drawCenter({ Window::Width() / 2,Window::Height() / 2 + 80 });
			}
			if (Input::KeyEnter.clicked) {	//リスタート準備
				if (HiScore_Flag == true) {
					writer.open(HiScore_Path, TextEncoding::UTF8);
					//writer.clear();
					writer.write(Format(score));
					writer.close();
					HiScore = score;
					HiScore_Flag = false;
				}
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

