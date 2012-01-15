//{{{ pre-written code
#include "YourPlayer.h"
//#include "Chara.h"

#include <stdlib.h>
#include <time.h>
#include <queue>
#include <cmath>
#include <cassert>
#include <fstream>
#include <cstdio>
#include <algorithm>
#include <vector>

#define REP(i,n) for(int i=0;i<(int)n;i++)
using namespace std;
//}}}

ofstream logfile;

int dir[4][2]={{-1,0},{1,0},{0,-1},{0,1}};
// LEFT, RIGHT, UP, DOWN
int width=-1,height=-1;

char field[M][M];
char (*c)[M];
char cs[8][M][M];

int remainingTime=-1,index=-1;
bool safeDog,adjDog;//味方犬と一緒にいるか、隣に敵犬がいるか
int state[4],staterem[4];
int samsc[4],dogsc[4],samdir[4],dogdir[4];
int rank[4];
int sam[4][2],dog[4][2];
int nearDog;

int (*dist)[M],(*prev)[M],(*point)[M];
int dists[8][M][M],prevs[8][M][M],points[8][M][M];
int index2,mikata;

int id;

//int dist[M][M],prev[M][M],point[M][M];
double prior[M][M];
const int INF=1000000000;
const double rate=0.9;
int xs,ys;
int itemNum=0,bigItemNum=0;
const int radii=4;
int baseX=radii,baseY=radii;
const int DEFAULTSHOGUNPOINT=105;
//const int DEFAULTSHOGUNPOINT=90;
int SHOGUNPOINT;

int capture=0;

const int F=11,K=4;
int ki,f0,f1;

bool samSOS=false,chaseSam=false;//侍からのSOS, 犬と侍が一緒になる(防御の布陣)べきか
//{{{ 入力を読む
void read(){
	// パラメータの読み込み
//	scanf("%d%d%d%d",&remainingTime,&index,&width,&height);
	cin >> remainingTime>>index>>width>>height;
	string line;
	getline(cin, line);
	// マップ情報の読み込み
	REP(y,height){
		getline(cin,line);
		REP(x,width)field[x][y] = line.at(x);
	}
	// キャラクタ情報の読み込み
	REP(i,4){
		// 侍情報の読み込み
//		scanf("%d%d%d%d%d%d",&samsc[i],&sam[i][0],&sam[i][1],&samdir[i],&state[i],&staterem[i]);
		cin >> samsc[i]>>sam[i][0]>>sam[i][1]>>samdir[i]>>state[i]>>staterem[i];
		// 犬情報の読み込み
//		scanf("%d%d%d%d",&dogsc[i],&dog[i][0],&dog[i][1],&dogdir[i]);
		cin >> dogsc[i] >> dog[i][0] >> dog[i][1]>> dogdir[i];
	}
}
	//}}}
//{{{ フィールド内判定
bool fieldInner(const int &x,const int &y,int i=index2){
	//フィールド内にあるかどうか
	if(not (0<=x and x<width and 0<=y and y<height))return false;
//	if(field[x][y]=='*')return false;
	if(cs[i][x][y]=='*')return false;
	return true;
}

//{{{ 移動物も含めてフィールド内にあるかどうか
//{{{ 自分
int inner2(int x,int y,bool isSam){
	if(not fieldInner(x,y))return 0;
	bool existSam=false,existDog=false;
	REP(i,4)if(i!=index and state[i]!=1 and x==sam[i][0] and y==sam[i][1])existSam=true;
	REP(i,4)if(i!=index and x==dog[i][0] and y==dog[i][1])existDog=true;
	int result;
	if(isSam){
		if(safeDog){
			if(existDog)result=2;
			else result=1;
		}else if(/*safeDog and */existSam and itemNum<=3){
			result=1;
		}else{
			if(state[index]==0 or (state[index]==2 and staterem[index]<=1)){
				if(itemNum<=3){
					if(existDog)result=2;
					else{
						if(samSOS and existSam)result=0;
						else result=1;
					}
				}else{
					if(existDog)result=2;
					else if(existSam)result=0;
					else result=1;
				}
			}else if(state[index]==1){
				result=1;
			}else if(state[index]==2){
				if(existSam)result=0;
				else result=1;
			}
		}
	}else{
		if(existDog)result=0;
		else result=1;
	}
	return result;
}
//}}}
//{{{ 敵
int inner(int x,int y,int index){
	bool isSam=index<4?true:false;
	if(index==index2 or index==mikata){
		return inner2(x,y,isSam);
	}
	if(not fieldInner(x,y,index))return 0;
	if(!isSam)index-=4;
	bool existSam=false,existDog=false;
	REP(i,4)if(i!=index and state[i]!=1 and x==sam[i][0] and y==sam[i][1])existSam=true;
	REP(i,4)if(i!=index and x==dog[i][0] and y==dog[i][1])existDog=true;
	int result;
	if(isSam){
		if(state[index]==0 or (state[index]==2 and staterem[index]<=1)){
			if(existDog)result=2;
			else if(existSam)result=0;
			else result=1;
		}else if(state[index]==1){
			result=1;
		}else if(state[index]==2){
			if(existSam)result=0;
			else result=1;
		}
	}else{
		if(existDog)result=0;
		else result=1;
	}
	return result;
}
//}}}
//}}}

//}}}
//{{{ setPrior function
/*
	void setPrior(int x,int y){
	bool valid=false;
	if(state[index]==0){
	REP(i,4){
	if(i==index)continue;
	}
	}
	double s=0.0;
	REP(d,4){
	int x1=x+dir[d][0],y1=y+dir[d][1];
	if(not fieldInner(x1,y1))continue;
	if(dist[x][y]+1!=dist[x1][y1])continue;
	setPrior(x1,y1);
	s+=prior[x1][y1];
	}
	s*=rate;
	double sc=0.0;
	if(c[x][y]=='p'){
	sc+=15.0;
	}else if(c[x][y]=='b'){
	sc+=10.0;
	}else if(c[x][y]=='s'){
	sc+=1.0;
	}
	if(x==dog[index][0] and y==dog[index][1]){
	sc+=dogsc[index];
	}
	if(state[index]==2){//SHOGUN!!
	REP(i,4){
	if(dog[i][0]==x and dog[i][1]==y)sc+=dogsc[i];
	}
	}
	prior[x][y]=s+sc;
	}
	*/
//}}}
//{{{ デバッグ用
void debugField(ofstream &logfile,const bool &isSam){
	//	logfile<<xs<<" "<<ys<<endl;
	if(capture){
		logfile<<"capture!!"<<endl;
	}
	logfile<<remainingTime<<" ";
	if(isSam){
		logfile<<"Sam"<<" ";
	}else{
		logfile<<"Dog"<<" ";
	}
	if(samSOS){
		logfile<<"SOS"<<" ";
	}else{
		logfile<<"SAFE"<<" ";
	}
	logfile<<endl;
	logfile<<baseX<<" "<<baseY<<endl;
	string c2[M][M];
	REP(y,height)REP(x,width)c2[x][y]+=c[x][y];
	REP(i,4){
		if(state[i]!=1)c2[sam[i][0]][sam[i][1]]+='A'+i;
		if(state[i]==2)c2[sam[i][0]][sam[i][1]]+="!";
		c2[dog[i][0]][dog[i][1]]+='0'+i;
	}
	REP(y,height){
		REP(x,width){
			logfile<<c2[x][y]<<"\t";
		}
		logfile<<endl;
	}
	logfile<<endl;
	REP(y,height){
		REP(x,width){
			if(dist[x][y]==INF)logfile<<"*"<<"\t";
			else logfile<<dist[x][y]<<"\t";
		}
		logfile<<endl;
	}
	logfile<<endl;
	REP(y,height){
		REP(x,width){
			if(point[x][y]==-INF)logfile<<"*"<<"\t";
			else logfile<<point[x][y]/10<<"\t";
		}
		logfile<<endl;
	}
	logfile<<endl;

}
//}}}

bool baseInner(int x,int y){
	return abs(baseX-x)<=radii and abs(baseY-y)<=radii;
}

int getPoint(int x,int y){//得られる得点
	if(not fieldInner(x,y))return 0;
	switch(field[x][y]){
		case '*': return 0;break;
		case 'p': return SHOGUNPOINT;break;
		case 'b': return 100;break;
		case 's': return 10;break;
		default: return 0;break;
	}
	return 0;
}

struct S{
	int x,y,prev,dist,poi;
	S(int x,int y,int prev,int dist,int poi):x(x),y(y),prev(prev),dist(dist),poi(poi){}
	bool operator<(const S &s) const {
		if(dist!=s.dist)return dist>s.dist;//INVERSE
		else return poi<s.poi;
	}
};

//{{{ 各プレーヤー・犬からの距離の計算
priority_queue<S> q;

void calcDist(int x0,int y0,int index,char c[M][M],int dist[M][M],int prev[M][M],int point[M][M]){
	while(!q.empty())q.pop();
	REP(x,M)REP(y,M)dist[x][y]=INF,prev[x][y]=-1,point[x][y]=-INF;
	dist[x0][y0]=0;point[x0][y0]=0;
	q.push(S(x0,y0,-1,0,0));
	while(!q.empty()){
		S t=q.top();q.pop();
		const int x=t.x,y=t.y,pre=t.prev;
		if(prev[x][y]!=-1)continue;
		prev[x][y]=pre;
		int in=inner(x,y,index);
		if(in==2 or in==0)continue;
		REP(di,4){
			const int x2=x+dir[di][0],y2=y+dir[di][1];
			int in=inner(x2,y2,index);
			if(in==0)continue;
			int p2=point[x][y]+getPoint(x2,y2),d2=dist[x][y]+1;
			if(dist[x2][y2]>d2 or (dist[x2][y2]==d2 and point[x2][y2]<p2)){
				dist[x2][y2]=d2;
				q.push(S(x2,y2,di,d2,p2));
				point[x2][y2]=p2;
			}
		}
	}
	/*
	logfile<<"debug: "<<endl;
	REP(y,height){
		REP(x,width){
			logfile<<((dist[x][y]==INF)?-1:dist[x][y])<<"\t";
		}
		logfile<<endl;
	}
	logfile<<endl;
	*/
}

void setDist(int i){
	char c2[M][M];
	REP(x,M)REP(y,M)c2[x][y]=c[x][y];
	if(i<4){//samurai

	}else{//dog
		i-=4;
	}
}

//}}}

int bias(const int &d){
	return d+1;
	if(d==1 or d==0)return 1;
	else return d*3;
}

bool isItem(int x,int y){
	if(field[x][y]=='s' or field[x][y]=='b' or field[x][y]=='p')return true;
	else return false;
}

bool isolate(int x,int y){//孤立しているか
	if(not isItem(x,y))return false;
	int ct=0;
	REP(di,4){
		int x1=x+dir[di][0],y1=y+dir[di][1];
		if(not fieldInner(x1,y1))continue;
		if(isItem(x1,y1))ct++;
	}
	return ct==0;
}

bool nextSquare(int x,int y){
	return abs(x-xs)+abs(y-ys)==1;
}

//{{{ 敵の犬や侍の存在判定
bool isEnemyDog(int x,int y){
	REP(i,4){
		if(i==index)continue;
		if(x==dog[i][0] and y==dog[i][1])return true;
	}
	return false;
}

bool isEnemySam(int x,int y){
	REP(i,4){
		if(i==index)continue;
		if(x==sam[i][0] and y==sam[i][1])return true;
	}
	return false;
}
//}}}
//{{{ 袋小路に入った場合の挟み撃ち作戦(仮)
//{{{ 第二案
int targetX=-1,targetY=-1,dogTarget=-1;
//犬が呼び出す
//(px, py): 味方の場所, (x,y): 探索回廊
void fukurokouji(int x,int y,int px,int py){
	int x0=px,y0=py;
	bool foundSam=false;
	int nx,ny;
	int id=-1;
	vector<pair<int,int> > vp;
	vp.push_back(make_pair(px,py));
	while(1){
		vp.push_back(make_pair(x,y));
		int ct=0;
		REP(p,4){
			int x1=x+dir[p][0],y1=y+dir[p][1];
			if(not inner(x1,y1,mikata))continue;
			if(x1==px and y1==py)continue;
			ct++;nx=x1;ny=y1;
		}
		if(ct==0)return;//味方侍による救援の必要なし
		else if(ct>=2)break;//袋小路終了
		if(id==-1){
			REP(i,4){
				if(i==index){
					if(sam[i][0]==nx and sam[i][1]==ny){
						if(targetX==-1 and targetY==-1){
							targetX=nx;targetY=ny;
						}
						return;
					}
				}else{
					if(sam[i][0]==x and sam[i][1]==y){
						id=i;
						break;
					}
				}
			}
		}
		if(id>=0){
			foundSam=true;
			int distMikata=dists[mikata][nx][ny],distEnemy=dists[id][nx][ny];
			if(foundSam and targetX==-1 and targetY==-1 and distMikata<distEnemy){
				targetX=nx;targetY=ny;
			}
		}
		if(isEnemyDog(x,y)){
			targetX=targetY=-1;
			return;
		}
		px=x,py=y,x=nx,y=ny;
	}
	logfile<<"fukurokouij: "<<x0<<" "<<y0<<"->"<<x<<" "<<y<<endl;;
	REP(i,vp.size()){
		logfile<<"("<<vp[i].first<<" "<<vp[i].second<<")";
	}
	logfile<<endl;
}
//}}}
//}}}

//距離に対するバイアス
double distBias(int me,int enemy){
	if(itemNum<=4)return 0.0;
	int diff=enemy-me;
	if(diff<0)return -INF;
	else{
//		return (double)0.0/(diff+10);
		return (double)100.0/(diff+5);
	}
}

//行き止まり判定
bool ikidomari(int x,int y){
	int ct=0;
	REP(di,4){
		int x1=x+dir[di][0],y1=y+dir[di][1];
		if(not fieldInner(x1,y1))continue;
		ct++;
	}
	if(ct<=1)return true;
	else return false;
}

double samPriority(int x,int y){
	if(not fieldInner(x,y))return (double)-INF;
	if(dist[x][y]>remainingTime/8)return (double)-INF;
	if(dist[x][y]<=0 or dist[x][y]==INF)return -1.0;
	//{{{ 犬に追われている場合には行き止まりに入らない
//	logfile<<"nearDog: "<<nearDog<<endl;
	REP(di,4){
		int x1=xs+dir[di][0],y1=ys+dir[di][1];
		if(not fieldInner(x1,y1))continue;
		if(x1!=x or y1!=y)continue;
		if(!safeDog and (state[index]==0 or staterem[index]<=2) and ikidomari(x1,y1) and nearDog<=2){
			return (double)-INF;
		}
	}
	//}}}
	/*
		if(capture and f[ki][f0][0]==x and f[ki][f0][1]==y){
		return INF;
		}
		*/
	double sc=0.0;
//	if(bigItemNum==0 and (x==1 or y==1 or x==height-2 or y==width-2) and isItem(x,y))sc+=1000.0;
//	if(isItem(x,y) and nextSquare(x,y) and isolate(x,y) and bigItemNum<=2)sc+=200.0;

	if(targetX>=0 and x==targetX and y==targetY){
		sc+=1000000.0;
		return INF;
	}
	/*
		if(c[x][y]=='p')sc+=SHOGUNPOINT;
		else if(c[x][y]=='b')sc+=100.0;
		else if(c[x][y]=='s'){
		sc+=10.0;
		}
		*/
	sc+=point[x][y];
	//	if((c[x][y]=='p' or c[x][y]=='b' or c[x][y]=='s') and baseInner(x,y))sc+=50.0;
	//犬から得点をもらいに行く
	if(x==dog[index][0] and y==dog[index][1] and remainingTime<dist[x][y]+20 and dist[x][y]<remainingTime+20){
		sc+=dogsc[index];
	}
	if(state[index]==2){//SHOGUN!!
		//敵犬から得点を奪いに行く
		/*
			REP(i,4){
			if(i==index)continue;
			if(staterem[index]>=10 and dogsc[i]>0 and dog[i][0]==x and dog[i][1]==y)sc+=dogsc[i];
			}
			*/
	}else{
		bool valid=true;
		REP(i,4){
			if(i==index)continue;
			if(dog[i][0]==x and dog[i][1]==y)valid=false;
		}
		if(!valid)return -1.0;
	}
	int nearestDist=INF;
	REP(i,4){
		if(i==index)continue;
		nearestDist=min(nearestDist,dists[i][x][y]);
	}
	if(isItem(x,y)){
		sc+=distBias(dist[x][y],nearestDist);
	}
	//	return sc/pow(2.0,dist[x][y]);
	return sc/bias(dist[x][y]);
}

double dogPriority(int i){
	//	if(i==index)return (double)INF;
	//	else return (double)-1.0;
	//ご主人様がお呼びだ
	if(samSOS or chaseSam){
		if(samSOS or (chaseSam and targetX==-1)){
			//		logfile<<"SOS"<<endl;
			if(i==index)return (double)INF;
			else return -1.0;
		}
	}
	int x=sam[i][0],y=sam[i][1];
	if(state[i]==1 or (state[i]==2 and dogsc[index]>0))return -INF;
	double w=1000.0;
	if(i==index){
		//得点を渡すべきか
		if(dogsc[index]>500.0 or 
				(dogsc[index]>0 and remainingTime<dist[x][y]+20 and dist[x][y]<remainingTime+20))
			return (double)INF;
		//		if(dogsc[index]==0 or dist[x][y]==0)return -1.0;
		return (double)dogsc[index];
	}else{
		if(rank[index]<=2){//自分が2位以上
			if(rank[i]==3)return w;//3位引きずり下ろし
			else return -w;
		}else if(rank[index]==3){//自分が3位
			if(rank[i]==2){//2位引きずり下ろし
				return w;
			}else{
				return -w;
			}
		}else{//自分が4位(最下位)
			if(rank[i]==2 or rank[i]==3){//2位, 3位引きずりおろし
				return w;
			}else{
				return -w;
			}
		}
		//得点を奪え
		if(state[i]==1)return (double)-INF;
		if(state[i]==2 and dogsc[index]>0)return -INF;
		double baseSc=(double)samsc[i];
		return baseSc*0.2/dists[index][sam[i][0]][sam[i][1]];
//		return baseSc*0.2/dist[sam[i][0]][sam[i][1]];
	}
}
//{{{ Dog Position
bool dogDanger(int x,int y){
	if(state[index]!=0 and staterem[index]>1)return false;
	if(dog[index][0]==x and dog[index][1]==y)return false;
	if(isEnemyDog(x,y))return true;
	if(safeDog and !isEnemyDog(x,y))return false;
	REP(d,4){
		int x1=x+dir[d][0],y1=y+dir[d][1];
		if(!fieldInner(x1,y1))continue;
		REP(i,4){
			if(i==index)continue;
			if(x1==dog[i][0] and y1==dog[i][1])return true;
		}
	}
	return false;
}
//}}}
//{{{ Escape
int findSamEscape(int x,int y){
	int p=-1;
	vector<int> ps;
	REP(d,4){
		int x1=x+dir[d][0],y1=y+dir[d][1];
		if(not fieldInner(x1,y1))continue;
		if(not dogDanger(x1,y1))ps.push_back(d);
	}
	random_shuffle(ps.begin(),ps.end());
	if(ps.size()>0)p=ps[0];
	return p;
}

int findDogEscape(int x,int y){
	int p=-1;
	vector<int> ps;
	REP(d,4){
		int x1=x+dir[d][0],y1=y+dir[d][1];
		if(!fieldInner(x1,y1))continue;
		REP(i,4){
			if(state[i]==2 and x1==sam[i][0] and y1==sam[i][1]){

			}else{
				ps.push_back(d);
			}
		}
	}
	random_shuffle(ps.begin(),ps.end());
	if(ps.size()>0)p=ps[0];
	return p;
}
//}}}

//{{{ Miscellaneous calc
//{{{ Player Struct 
struct Player{
	int i,sc;
	Player(int i,int sc):i(i),sc(sc){}
	bool operator<(const Player &s) const {
		return sc<s.sc;
	}
};
//}}}
void calcMiscellaneous(){
	//{{{ calc near Dog
	nearDog=INF;
	REP(i,4){
		if(i==index)continue;
		//				logfile<<dist[dog[i][0]][dog[i][1]]<<endl;
		nearDog=min(nearDog,dist[dog[i][0]][dog[i][1]]);
	}
	//}}}
	//{{{ calc adjDog
	adjDog=false;
	REP(d,4){
		int x1=sam[index][0]+dir[d][0],y1=sam[index][1]+dir[d][1];
		if(not fieldInner(x1,y1))continue;
		REP(i,4){
			if(i==index)continue;
			if(dog[i][0]==x1 and dog[i][1]==y1)adjDog=true;
		}
	}
	//}}}
	//{{{ calc rank
	vector<Player> vp;
	REP(i,4)vp.push_back(Player(i,samsc[i]));
	sort(vp.begin(),vp.end());
	reverse(vp.begin(),vp.end());
	REP(i,4)rank[vp[i].i]=i+1;
	//}}}
	logfile<<"rank: "<<endl;
	REP(i,4){
		logfile<<rank[i]<<" ";
	}
	logfile<<endl;
}
//}}}

string direction(const int &p){
	if(p==0)return "LEFT";
	else if(p==1)return "RIGHT";
	else if(p==2)return "UP";
	else if(p==3)return "DOWN";
	else return "NONE";
}

int attack(const bool &isSam){
	if(isSam){
		if(state[index]==2 and staterem[index]>1){
			REP(p,4){
				int x1=xs+dir[p][0],y1=ys+dir[p][1];
				REP(i,4){
					if(i==index)continue;
					if(dogsc[i]>0 and x1==dog[i][0] and y1==dog[i][1])return p;
				} }
		}
	}else{
		if(!adjDog){
			REP(p,4){
				int x1=xs+dir[p][0],y1=ys+dir[p][1];
				bool found=false;
				REP(i,4){
					if(i==index)continue;
					if(state[i]!=0)continue;
					if(x1==sam[i][0] and y1==sam[i][1]){
						return p;
					}
				}
			}
		}
	}
	return -1;
}


string calcDir(const bool &isSam){
	int p=attack(isSam);
	if(p>=0)return direction(p);
	int x0=sam[index][0],y0=sam[index][1];
	if(isSam){
		double maxPriority=-1.0;
		//		bool baseHit=false;
		REP(x,width)REP(y,height){
			if(not fieldInner(x,y))continue;
			double pr=samPriority(x,y);
			if(pr>maxPriority)maxPriority=pr,x0=x,y0=y;
		}
		//		if(!baseHit)baseX=x0,baseY=y0;
		//{{{ 侍のSOS
		samSOS=chaseSam=false;
		if(itemNum<=2)chaseSam=true;
		int myDogDist=dist[dog[index][0]][dog[index][1]];
		if((state[index]==0 and nearDog<=4) or 
				(state[index]==2 and nearDog<=2)){
			samSOS=true;
			if(state[index]==0)SHOGUNPOINT=300;
		}
		//SOSの時の侍の行動
		if((samSOS and nearDog==0 and staterem[index]<=8) or 
				(chaseSam and dogsc[index]>0 and remainingTime<myDogDist+20 and myDogDist<remainingTime+20)
		  ){
			if((state[index]==0 or state[index]==2) and !safeDog){
				x0=dog[index][0],y0=dog[index][1];
			}
		}
		//}}}
	}else{
		double maxPriority=-1.0;
		//		if(samSOS)logfile<<d<<endl;
		REP(i,4){
			if(i==index and dogsc[index]==0)continue;
			double pr=dogPriority(i);
			if(pr>maxPriority)maxPriority=pr,x0=sam[i][0],y0=sam[i][1];
		}
		targetX=targetY=-1;
		logfile<<"=== BEGIN ====="<<endl;
		REP(p,4){
			int x1=xs+dir[p][0],y1=ys+dir[p][1];
			if(not fieldInner(x1,y1))continue;
			fukurokouji(x1,y1,xs,ys);
		}
		logfile<<"=== END ========"<<endl;
	}
	if(targetX>=0){
		logfile<<"capture: "<<remainingTime<<" "<<targetX<<" "<<targetY<<endl;
	}
	//	logfile<<x0<<" "<<y0<<endl;
	//	logfile<<remainingTime<<endl;
	//	debugField(logfile,isSam);
	if(x0<0 or y0<0)return "NONE";
	if(x0==xs and y0==ys)return "NONE";
	//	logfile<<x0<<" "<<y0<<endl;
	//	logfile<<"state: "<<state[index]<<endl;
	p=-1;
	while(1){
		if(prev[x0][y0]==-1)break;
		p=prev[x0][y0];
		x0=x0-dir[p][0];y0=y0-dir[p][1];
	}
	return direction(p);
}

void calc(const bool &isSam){
	static bool isFirstCalled = true;
	SHOGUNPOINT=DEFAULTSHOGUNPOINT;
	// initialize random method when first called
	if (isFirstCalled) {
		srand((unsigned int)time(NULL));
		isFirstCalled = false;
		logfile.open("out.txt");
	}
	read();
	logfile<<isSam<<endl;
	//	logfile<<"staterem: "<<staterem[index]<<endl;
	safeDog=(sam[index][0]==dog[index][0] and sam[index][1]==dog[index][1]);
	//	if(safeDog)SHOGUNPOINT=30;
	itemNum=bigItemNum=0;
	REP(x,width)REP(y,height)if(isItem(x,y))itemNum++;
	REP(x,width)REP(y,height)if(field[x][y]=='b')bigItemNum++;
	//{{{ copy each
	REP(i,8)REP(x,width)REP(y,height)cs[i][x][y]=field[x][y];
	//	logfile<<"fieldInner: "<<fieldInner(1,1)<<" "<<fieldInner(1,2)<<" "<<fieldInner(2,1)<<endl;
	index2=isSam?index:index+4;
	mikata=isSam?index+4:index;
	c=cs[index2],dist=dists[index2],prev=prevs[index2],point=points[index2];
	//}}}
	//	if(state[index]==2 and staterem[index]>=10)SHOGUNPOINT=30;
	if(isSam)xs=sam[index][0],ys=sam[index][1];
	else xs=dog[index][0],ys=dog[index][1];
	//{{{ invalidate dangerous block
	if(isSam){
		REP(d,4){
			int x1=xs+dir[d][0],y1=ys+dir[d][1];
			if(not fieldInner(x1,y1))continue;
			if(!isEnemyDog(x1,y1) and dogDanger(x1,y1) and c[x1][y1]!='p')c[x1][y1]='*';
		}
	}else{
		if(dogsc[index]>0){
			REP(d,4){
				int x1=xs+dir[d][0],y1=ys+dir[d][1];
				if(not fieldInner(x1,y1))continue;
				REP(i,4){
					if(i==index)continue;
					if(state[i]==2 and x1==sam[i][0] and y1==sam[i][1])c[x1][y1]='*';
				}
				REP(d2,4){
					int x2=x1+dir[d][0],y2=y1+dir[d][1];
					if(not fieldInner(x2,y2))continue;
					REP(i,4){
						if(i==index)continue;
						if(state[i]==2 and staterem[i]>1 and x2==sam[i][0] and y2==sam[i][1])c[x1][y1]='*';
					}
				}
			}
		}
		bool found=false;
		REP(d,4){
			int x1=xs+dir[d][0],y1=ys+dir[d][1];
			if(sam[index][0]==x1 and sam[index][1]==y1 and ikidomari(x1,y1))c[x1][y1]='*',found=true;
		}
		if(found){
			cout<<"NONE"<<endl;
			return;
		}
	}
	//}}}
	if((state[index]==2 and staterem[index]*8>=remainingTime) or bigItemNum==0){
		SHOGUNPOINT=10;
	}
	REP(i,4){
		calcDist(sam[i][0],sam[i][1],i,cs[i],dists[i],prevs[i],points[i]);
	}
	REP(i,4){
		int j=i+4;
		calcDist(dog[i][0],dog[i][1],j,cs[j],dists[j],prevs[j],points[j]);
	}
	//calcDist(xs,ys,index2,c,dist,prev,point);
	//{{{ calc all dist
	/*
		REP(i,4){
		REP(x,width)REP(y,width)c2[x][y]=c[x][y];
		if(i==index and isSam){
		REP(d,4){
		int x1=xs+dir[d][0],y1=ys+dir[d][1];
		if(not fieldInner(x1,y1))continue;
		if(!isDog(x1,y1) and dogDanger(x1,y1) and c[x1][y1]!='p')c[x1][y1]='*';
		}
		}
		calcDist(sam[i][0],sam[i][1],isSam,c2,dists[i],prevs[i],points[i]);
		}
		REP(i,4){
		REP(x,width)REP(y,width)c2[x][y]=c[x][y];
		calcDist(dog[i][0],dog[i][1],isSam,c2,dists[4+i],prevs[4+i],points[4+i]);
		}
		if(isSam){
		dist=dists[index],prev=prevs[index],point=points[index];
		}else{
		dist=dists[index+4],prev=prevs[index+4],point=points[index+4];
		}
		*/
	//}}}
	//	fukurokouji();
	//	if(isSam)setPrior(xs,ys);
	calcMiscellaneous();
	string ans=calcDir(isSam);
	cout<<ans<<endl;
	return;
}
