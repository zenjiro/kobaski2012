using namespace std;

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

ofstream logfile;

int dir[4][2]={{-1,0},{1,0},{0,-1},{0,1}};
// LEFT, RIGHT, UP, DOWN
const int M=32;
int width=-1,height=-1;
char c[M][M];
int remainingTime=-1,index=-1;
bool safeDog;
int state[4],staterem[4];
int samsc[4],dogsc[4],samdir[4],dogdir[4];
int sam[4][2],dog[4][2];
int dist[M][M],prev[M][M];
double prior[M][M];
const int INF=1000000000;
const double rate=0.9;
int xs,ys;
int itemNum=0;

bool samSOS=false;

//{{{ my getline function
void getline(char *p){
	char c;
	do{
		c=getchar();
	}while(c=='\n' or c=='\r' or c==EOF);
	do{
		*p=c;p++;
		c=getchar();
	}while(!(c=='\n' or c=='\r' or c==EOF));
	p='\0';
}
//}}}
//{{{ read
void YourPlayer::read(ofstream &logfile){
	// パラメータの読み込み
	scanf("%d%d%d%d",&remainingTime,&index,&width,&height);
//	cin >> remainingTime>>index>>width>>height;
	
	char in[M];

	string line;

//	getline(in);
	getline(cin, line);
//	getchar();
//	logfile<<in<<endl;

	// マップ情報の読み込み
	REP(y,height){
//		getline(in);
		getline(cin,line);
		REP(x,width)c[x][y] = line.at(x);
//		in[width]='\0';
//		logfile<<line<<endl;
	}

	// キャラクタ情報の読み込み
	REP(i,4){
		// 侍情報の読み込み
//		string directionS;
//		string stateS;

		scanf("%d%d%d%d%d%d",&samsc[i],&sam[i][0],&sam[i][1],&samdir[i],&state[i],&staterem[i]);
//		cin >> samsc[i]>>sam[i][0]>>sam[i][1];
//		logfile<<samsc[i]<<" "<<sam[i][0]
		//{{{ samrai direction
		/*
		cin >> directionS;
		if (directionS == "RIGHT") {
			samdir[i]=0;
		} else if (directionS == "UP") {
			samdir[i]=1;
		} else if (directionS == "LEFT") {
			samdir[i]=2;
		} else if (directionS == "DOWN") {
			samdir[i]=3;
		} else {
			samdir[i]=-1;
		}
		*/
		//}}}
//		cin>>samdir[i]>>state[i];
		/*
		cin >> stateS;
		logfile<<"stateS: "<<stateS<<endl;
		if (stateS == "NORMAL") {
			state[i]=0;
		} else if (stateS == "INVISIBLE") {
			state[i]=1;
		} else if (stateS == "SHOGUN") {
			state[i]=2;
		}
		*/
//		cin >> staterem[i];

		// 犬情報の読み込み
		scanf("%d%d%d%d",&dogsc[i],&dog[i][0],&dog[i][1],&dogdir[i]);
//		cin >> dogsc[i] >> dog[i][0] >> dog[i][1]>> dogdir[i];
		//{{{ dog direction
		/*
		cin >> directionD;
		if (directionD == "RIGHT") {
			dogdir[i]=0;
		} else if (directionD == "UP") {
			dogdir[i]=1;
		} else if (directionD == "LEFT") {
			dogdir[i]=2;
		} else if (directionD == "DOWN") {
			dogdir[i]=3;
		} else {
			dogdir[i]=-1;
		}
		*/
		//}}}
	}
	safeDog=(sam[index][0]==dog[index][0] and sam[index][1]==dog[index][1]);
}
	//}}}

struct S{
	int x,y,p;
	S(int x,int y,int p):x(x),y(y),p(p){}
};

bool fieldInner(const int &x,const int &y){
	//フィールド内にあるかどうか
	if(not (0<=x and x<width and 0<=y and y<height))return false;
	if(c[x][y]=='*')return false;
	return true;
}

int inner(const int &x,const int &y,const bool &isSam){
	//移動物も含めてフィールド内にあるかどうか
	if(not fieldInner(x,y))return 0;
	bool existSam=false,existDog=false;
	REP(i,4)if(i!=index and x==sam[i][0] and y==sam[i][1])existSam=true;
	REP(i,4)if(i!=index and x==dog[i][0] and y==dog[i][1])existDog=true;
	int result;
	if(isSam){
		if(state[index]!=0){
			if(existSam)result=0;
			else result=1;
		}else{
			if(safeDog){
				if(existDog)result=2;
				else result=1;
			}else{
				if(existDog)result=2;
				else if(existSam)result=0;
				else result=1;
			}
		}
	}else{
		if(existDog)result=0;
		else result=1;
	}
	return result;
}

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
//{{{ for debug
void debugField(ofstream &logfile){
	logfile<<xs<<" "<<ys<<endl;
	REP(y,height){
		REP(x,width){
			if(dist[x][y]==INF)logfile<<"*"<<" ";
			else logfile<<dist[x][y]<<" ";
		}
		logfile<<endl;
	}
	logfile<<endl;
	REP(y,height){
		REP(x,width){
			logfile<<c[x][y]<<" ";
		}
		logfile<<endl;
	}
	logfile<<endl;
}
//}}}

queue<S> q;

void YourPlayer::calcDist(int x0,int y0,const bool &isSam){
	while(!q.empty())q.pop();
	REP(x,M)REP(y,M)dist[x][y]=INF,prev[x][y]=-1;
	q.push(S(x0,y0,-1));
	dist[x0][y0]=0;
	while(!q.empty()){
		S t=q.front();q.pop();
		const int x=t.x,y=t.y,p=t.p;
		if(prev[x][y]!=-1)continue;
		prev[x][y]=p;
		char in=inner(x,y,isSam);
		if(in==2)continue;
		REP(d,4){
			const int x2=x+dir[d][0],y2=y+dir[d][1];
			char in=inner(x2,y2,isSam);
			if(in==0)continue;
			if(dist[x2][y2]>dist[x][y]+1){
				dist[x2][y2]=dist[x][y]+1;
				q.push(S(x2,y2,d));
			}
		}
	}
}

double samPriority(int x,int y){
	if(dist[x][y]<=0 or dist[x][y]==INF)return -1.0;
	double sc=0.0;
	if(c[x][y]=='p'){
		sc+=20.0;
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
			if(i==index)continue;
			if(dogsc[i]>0 and dog[i][0]==x and dog[i][1]==y)sc+=dogsc[i];
		}
	}else{
		bool valid=true;
		REP(i,4){
			if(i==index)continue;
			if(dog[i][0]==x and dog[i][1]==y)valid=false;
		}
		if(!valid)return -1.0;
	}
	return sc/dist[x][y];
}

double dogPriority(int i){
	//	if(i==index)return (double)INF;

	//	else return (double)-1.0;
	//ご主人様が危ない
	if(samSOS){
		//		logfile<<"SOS"<<endl;
		if(i==index)return (double)INF;
		else return -1.0;
	}
	//得点を奪え
	int x=sam[i][0],y=sam[i][1];
	if(i==index){
		if(dogsc[index]>0 and dist[x][y]<remainingTime)return (double)INF;
		if(dogsc[index]==0 or dist[x][y]==0)return -1.0;
		return (double)dogsc[index];
	}else{
		if(state[i]==2 and dogsc[index]>0)return -1.0;
		double baseSc=(double)samsc[i]+100;
		return baseSc*0.2/dist[sam[i][0]][sam[i][1]];
	}
}

bool dogDanger(int x,int y){
	if(state[index]!=0)return false;
	REP(i,4){
		if(i==index)continue;
		if(x==dog[i][0] and y==dog[i][1])return true;
	}
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

bool isDog(int x,int y){
	REP(i,4){
		if(i==index)continue;
		if(x==dog[i][0] and y==dog[i][1])return true;
	}
	return false;
}

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

string YourPlayer::calcDir(const bool &isSam){
	int x0=sam[index][0],y0=sam[index][1];
	if(isSam){
		double maxPriority=-1.0;
		itemNum=0;
		REP(x,width)REP(y,height){
			if(not fieldInner(x,y))continue;
			double pr=samPriority(x,y);
			//			if(!(c[x][y]=='s' or c[x][y]=='b'))continue;
			if(pr>maxPriority)maxPriority=pr,x0=x,y0=y;
			if(c[x][y]=='s' or c[x][y]=='b' or c[x][y]=='p')itemNum++;
		}
		samSOS=false;
		if(itemNum==0)samSOS=true;
		int myDogDist=dist[dog[index][0]][dog[index][1]];
		REP(i,4){
			if(i==index)continue;
			//				logfile<<dist[dog[i][0]][dog[i][1]]<<endl;
			int d=dist[dog[i][0]][dog[i][1]];
			if((state[index]==0 and d<=6) or (state[index]>=1 and d<=1 and staterem[index]>myDogDist)){
				samSOS=true;
			}
		}
		/*
			if(state[index]==0 and samSOS and (dog[index][0]!=sam[index][0] or dog[index][1]!=sam[index][1])){
			x0=dog[index][0],y0=dog[index][1];
			}
			*/
	}else{
		double d=dogPriority(index), maxPriority=-1.0;
		//		if(samSOS)logfile<<d<<endl;
		if(d>=300.0){
			x0=sam[index][0],y0=sam[index][1];
		}else{
			int minDist=INF;
			REP(i,4){
				if(i==index and dogsc[index]==0)continue;
				double pr=dogPriority(i);
				if(pr>maxPriority)maxPriority=pr,x0=sam[i][0],y0=sam[i][1];
			}
		}
	}
	logfile<<remainingTime<<" "<<samSOS<<endl;
	if(x0<0 or y0<0)return "NONE";
	if(x0==xs and y0==ys)return "NONE";
	//	logfile<<x0<<" "<<y0<<endl;
	//	logfile<<"state: "<<state[index]<<endl;
	//	debugField(logfile);
	int p=-1;
	while(1){
		if(prev[x0][y0]==-1)break;
		p=prev[x0][y0];
		x0=x0-dir[p][0];y0=y0-dir[p][1];
	}
	//	assert(x0==xs and y0==ys);
	int x1=x0+dir[p][0],y1=y0+dir[p][1];
	if(isSam){
		if(state[index]==0){
			if(state[index]==0 and c[x1][y1]!='p' and !safeDog){
				//			if(dogDanger(x1,y1))p=findSamEscape(xs,ys);
				if(dogDanger(x1,y1))return "NONE";
			}else if(isDog(x1,y1)){
				p=findSamEscape(xs,ys);
				return "NONE";
			}
		}
	}else{
		bool valid=true;
		REP(i,4){
			if(i==index)continue;
			if(state[i]==2 and sam[i][0]==x1 and sam[i][1]==y1){
				valid=false;
			}
		}
		if(not valid)p=findDogEscape(xs,ys);
	}
	/*
		if(isSam){
		p=-1;
		double maxPr=-0.5;
		REP(d,4){
		int x1=xs+dir[d][0],y1=ys+dir[d][1];
		if(not fieldInner(x1,y1))continue;
		if(maxPr<prior[x1][y1])maxPr=prior[x1][y1],p=d;
		}
		}
		*/
	if(p==0)return "LEFT";
	else if(p==1)return "RIGHT";
	else if(p==2)return "UP";
	else if(p==3)return "DOWN";
	else return "NONE";
}

void YourPlayer::calc(const bool &isSam){
	static bool isFirstCalled = true;
	// initialize random method when first called
	if (isFirstCalled) {
		srand((unsigned int)time(NULL));
		isFirstCalled = false;
		logfile.open("out.txt");
	}
	//	logfile<<"HELLO WORLD"<<endl;
	read(logfile);
	if(isSam)xs=sam[index][0],ys=sam[index][1];
	else xs=dog[index][0],ys=dog[index][1];
	calcDist(xs,ys,isSam);
	//	if(isSam)setPrior(xs,ys);
	string ans=calcDir(isSam);
	cout<<ans<<endl;
	return;
}


