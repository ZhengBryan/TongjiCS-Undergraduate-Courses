#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

// ��ʾ�������ݵ���������
// ˵�������������㷨��ʽ��ͬ�������ִ�С��ͬʱ����ѡ����ͬ����ĸ��Ϊ����۲죬�ڱȽϴ�Сʱ��������ͬ�����һ������ĸ��ASCII����Ϊ�Ƚ�����
struct ElemType {
	int num;
	char letter;

	bool operator< (const ElemType& that) const
	{
		// ѡ���㷨�����ȶ���
		// ��������ͬ��ǰ���±Ƚ���ĸ�ܹ�����������ͬ ����۲�
		if (num == that.num)
			return letter < that.letter;
		return num < that.num;
	}

	bool operator> (const ElemType& that) const
	{
		if (num == that.num)
			return letter > that.letter;
		return num > that.num;
	}
};

class Solve {
private:
	// �� nums[l] ��λ����ȷ��λ����
	int partition(vector<ElemType>& nums, int l, int r)
	{
		int i = l - 1, j = r + 1;

		ElemType x = nums[l];
		while (true) {
			while (nums[++i] < x && i < r)
				;
			while (nums[--j] > x)
				;
			if (i >= j)
				break;

			swap(nums[i], nums[j]);
		}

		return j;
	}

	// �������� ���ڷ���3
	int partition(vector<ElemType>& nums, int l, int r, ElemType x)
	{
		int i = l - 1, j = r + 1;

		while (true) {
			while (nums[++i] < x && i < r)
				;
			while (nums[--j] > x)
				;
			if (i >= j)
				break;

			swap(nums[i], nums[j]);
		}

		return j;
	}

	int randomizedPartition(vector<ElemType>& nums, int l, int r)
	{
		// ���ѡ��һ����
		int i = rand() % (r - l + 1) + l;
		swap(nums[l], nums[i]);
		return partition(nums, l, r);
	}

	// ���ֵݹ�Ĳ���
	ElemType randomizedSelect(vector<ElemType>& nums, int l, int r, int k)
	{
		if (l == r)
			return nums[l];
		int i = randomizedPartition(nums, l, r);
		// j ��ʾ i ������ l �� r ���ǵ� j ��
		int j = i - l + 1;
		
		if (k <= j)
			return randomizedSelect(nums, l, i, k);
		else
			return randomizedSelect(nums, i + 1, r, k - j);
	}


	// ѡ����λ������Ŧ�İ汾
	ElemType medianSelect(vector<ElemType>& arr, int l, int r, int k)
	{
		
		// �����㹻�� ֱ�������ҽ��
		if (r - l < 75) {
			// ð������
			sort(arr.begin() + l, arr.begin() + r + 1);
			return arr[l + k - 1];
		}

		for (int i = 0; i <= (r - l - 4) / 5; i++) {
			// ��������λ��
			sort(arr.begin() + l + 5 * i, arr.begin() + l + 5 * i + 4);
			// �ҵ���λ������������ͷ
			swap(arr[l + i], arr[l + 5 * i + 2]);
		}

		// �����ݹ�����λ������λ��
		ElemType x = medianSelect(arr, l, l + (r - l - 4) / 5, (r - l - 4) / 10);

		int i = partition(arr, l, r, x);

		int j = i - l + 1;

		if (k <= j)
			return medianSelect(arr, l, i, k);
		else
			return medianSelect(arr, i + 1, r, k - j);
	}

public:
	// ����1 ͨ����ʵ��
	ElemType solve1_heap(vector<ElemType> nums, int k)
	{
		// ��ǰ k ��������һ������ѣ�ʱ�临�Ӷ�Ϊ O(k)
		make_heap(nums.begin(), nums.begin() + k);

		// ����֮�����������ά������ѣ�ʱ�临�Ӷ�Ϊ O((n-k)logk)
		for (int i = k; i < (int)nums.size(); i++) {
			// �����ڶѶ�Ԫ��������
			if (nums[i] > nums[0])
				continue;
			// ���¶�
			pop_heap(nums.begin(), nums.begin() + k);
			nums[k - 1] = nums[i];
			push_heap(nums.begin(), nums.begin() + k);
		}
		return nums[0];
	}

	// ����2 �����������ѡ��
	ElemType solve2_randomized(vector<ElemType> nums, int k)
	{
		return randomizedSelect(nums, 0, nums.size() - 1, k);
	}

	// ����3 ��λ������ѡ��
	ElemType solve3_median(vector<ElemType> nums, int k)
	{
		return medianSelect(nums, 0, nums.size() - 1, k);
	}

};

int main()
{
	Solve s;
	int n, k;
	time_t t_start, t_end;
	vector<ElemType> nums;

	srand((unsigned)time(nullptr));

	for (int j = 1; j <= 10; j++) {
		// �Ӳ��������ж������ȷ��
		ElemType answer;
		// ���ļ���
		ifstream infile("in" + to_string(j) + ".txt", ios::in);		// in.txt �����������������Ϣ
		ifstream infile2("out" + to_string(j) + ".txt", ios::in);	// out.txt ����������������Ϣ(��ȷ��)
		if (!infile.is_open() || !infile2.is_open())
			return 0;
		infile >> n >> k;
		nums.clear();
		for (int i = 0; i < n; i++) {
			ElemType e;
			infile >> e.letter >> e.num;
			nums.push_back(e);
		}
		infile2 >> answer.letter >> answer.num;

		infile.close();
		infile2.close();

		cout << "-- ������Ե�" << j << "��������� --" << endl;
		cout << "��ȷ���� : " << answer.letter << " " << answer.num << endl;


		// �ѽ��д�뵽�ļ�
		for (int i = 1; i <= 3; i++) {
			string outfilename = "out" + to_string(j) + "_solution" + to_string(i) + ".txt";
			ofstream outfile(outfilename, ios::out);
			if (!outfile.is_open())
				return 0;
			t_start = clock();
			ElemType newans;
			switch (i) {
				case 1:
					newans = s.solve1_heap(nums, k);
					break;
				case 2:
					newans = s.solve2_randomized(nums, k);
					break;
				case 3:
					newans = s.solve3_median(nums, k);
					break;
			}
			t_end = clock();
			outfile << newans.letter << " " << newans.num;
			cout << "����" << i << "�õ��� : " << newans.letter << " " << newans.num << ", ��ʱ" << double(t_end - t_start) / CLOCKS_PER_SEC << "s" << endl;
			outfile.close();

		}
		cout << endl << endl;
	}
		
	return 0;
}