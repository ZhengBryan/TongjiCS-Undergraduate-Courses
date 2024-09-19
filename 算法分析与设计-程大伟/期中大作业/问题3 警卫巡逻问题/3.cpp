#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
using namespace std;

// �洢�ڵ����Ϣ
struct node {
    // ����
    int x, y;
    // �ڵ�����
    char id;
};

class Solution {
private:
    // ������
    int n;
    // �����Ϣ
    vector<node> nodeList;
    // ��¼��ɵ���������Ϣ
    vector<vector<int> > s;

    // �������ŷ�Ͼ���
    double getEucDistance(int i, int j)
    {
        return sqrt((nodeList[i].x - nodeList[j].x) * (nodeList[i].x - nodeList[j].x) + (nodeList[i].y - nodeList[j].y) * (nodeList[i].y - nodeList[j].y));
    }

public:
    // ��ʼ�� ������Ϣ
    void init()
    {
        cin >> n;

        s.resize(n);
        for (int i = 0; i < n; i++)
            s[i].resize(n);
        
        for (int i = 0; i < n; i++) {
            node element;
            cin >> element.id >> element.x >> element.y;
            nodeList.push_back(element);
        }
    }

    // ������С����Ѳ�߳ɱ�
    double getLowestCost()
    {
        // dp[i][j] ��ʾ͹����δ� i �� j ��С�ľ���Ѳ�߳ɱ�
        vector<vector<double> > dp(n, vector<double>(n, 0));

        // r Ϊ j �� i �Ĳ�ֵ
        for (int r = 2; r <= n; r++) {
            for (int i = 0; i < n - r; i++) {
                int j = i + r;
                dp[i][j] = INFINITY;
                // k �� i �� j ����
                for (int k = i + 1; k < j; k++) {
                    double u = dp[i][k] + getEucDistance(i, k) + getEucDistance(k, j) + getEucDistance(i, j) + dp[k][j];
                    if (u < dp[i][j]) {
                        dp[i][j] = u;
                        // �洢���ַ���
                        s[i][j] = k;
                    }
                }
            }
        }

        return dp[0][n - 1];
    }

    // �������Ѳ��·�߷���
    void printSolution()
    {
        vector<pair<int, int> > solutionList;
        solutionList.push_back({ 0, n - 1 });
        while (solutionList.size()) {
            pair<int, int> cur = solutionList.back();
            solutionList.pop_back();
            // �Ѿ�������ֹ�����
            if (cur.first + 1 == cur.second)
                continue;
            cout << nodeList[cur.first].id << nodeList[cur.second].id << nodeList[s[cur.first][cur.second]].id << endl;
            solutionList.push_back({ cur.first, s[cur.first][cur.second] });
            solutionList.push_back({ s[cur.first][cur.second], cur.second });
        }
    }
};


int main()
{

    Solution s;
    s.init();
    cout << s.getLowestCost() << endl;
    s.printSolution();

    return 0;
}

