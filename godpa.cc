#include <iostream>
#include <vector>
#include <queue>

using namespace std;

bool PosOk(vector<vector<char>>& vvc, int i, int j)
{
	return i >= 0 && j >= 0 && i < vvc.size() && j < vvc[i].size();
}

int bfs(vector<vector<char>>& vvc, pair<int, int> p)
{
	queue<pair<pair<int, int>, int>> q;
	q.push(make_pair(p, 0));

	while(!q.empty())
	{
		int i = q.front().first.first;
		int j = q.front().first.second;
		int d = q.front().second;
		q.pop();
		for(int di = -1; di <= 1; di++)
			for(int dj = -1; dj <= 1; dj++)
			{
				if(abs(di) == abs(dj)) continue;
				int i2 = i + di;
				int j2 = j + dj;
				if(!PosOk(vvc, i2, j2) continue;
				if(vvc[i2][j2] == 'B') return d+1;
				if(vvc[i2][j2] != 'X')
				{
					vvc[i2]][j2] = 'X';
					q.push_back(make_pair(make_pair(i2, j2), d+1));
				}
			}
	}
	return -1;
}

int dfs(vector<vector<char>>& vvc, pair<int, int> p)
{
	stack<pair<pair<int, int>, int>> q;
	q.push(make_pair(p, 0));

	while(!q.empty())
	{
		int i = q.top().first.first;
		int j = q.top().first.second;
		int d = q.top().second;
		q.pop();
		for(int di = -1; di <= 1; di++)
			for(int dj = -1; dj <= 1; dj++)
			{
				if(abs(di) == abs(dj)) continue;
				int i2 = i + di;
				int j2 = j + dj;
				if(!PosOk(vvc, i2, j2) continue;
				if(vvc[i2][j2] == 'B') return d+1;
				if(vvc[i2][j2] != 'X')
				{
					vvc[i2]][j2] = 'X';
					q.push(make_pair(make_pair(i2, j2), d+1));
				}
			}
	}
	return -1;
}

void permutations(vector<int>& v, vector<bool>& used, vector<int>& sol)
{
	if(sol.size() == v.size())
	{
		for (int i : sol) cout << v[i];
		cout << endl;
	}
	else
	{
		for(int i = 0; i < v.size(); i++)
		{
			if(!used[i] && sol.size() <= 1 || v[i]+sol[sol.size()-2] <= 2*sol.back())
			{
				used[i] = true;
				sol.push_back(i);
				permutations(v, used, sol);
				sol.pop_back();
				used[i] = false;
			}
		}
	}
}

typedef pair<int,int> P;
typedef vector<P> VP;
typedef vector<VP> Graph;

void dijkstra(int vi, int vf, const Graph& G)
{
  vector<int> dist(G.size(),INF);
  vector<int> pare(G.size());
  priority_queue<P> Q; //distancia, vertex
  dist[vi] = 0;
  Q.push(make_pair(0,vi));

  if(vi == vf)
  {
    cout<< "c(" << vi << "," << vf << ") = " << 0 << endl;
    return;
  }
  while(not Q.empty())
  {
    P p = Q.top(); Q.pop();
    int d = -p.first;
    int x = p.second;

    if(x == vf)
    {
      cout<< "c(" << vi << "," << vf << ") = " << d << endl;
      stack<int> pila;
      while(x != vi)
      {
        pila.push(x);
        x = pare[x];
      }
      cout<< vi;
      while(not pila.empty())
      {
        cout<<' '<<pila.top();
        pila.pop();
      }
      cout<<endl; return;
    }
    for(P arc : G[x])
    {
      int d2 = d+arc.first;
      int y = arc.second;
      if(d2 < dist[y])
      {
        dist[y] = d2;
        pare[y] = x;
        Q.push(make_pair(-d2,y));
      }
    }
  }
  cout<< "c(" << vi << "," << vf << ") = +oo" << endl;´´
}

int main()
{
  int n, m;
  cin>>n>>m;
  Graph G(n);

  while(m > 0)
  {
    int x,y,z;
    cin>>x>>y>>z;
    G[x].push_back(make_pair(z,y));
    G[y].push_back(make_pair(z,x));//si es no dirigido
    --m;
  }

  int x,y;
  while(cin>>x>>y)
  {
    dijkstra(x,y,G);
  }
}
