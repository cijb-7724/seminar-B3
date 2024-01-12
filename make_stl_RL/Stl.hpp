/*
STL�f�[�^�N���X
���ۂ�STL�̕ێ��̎d���Ə����Ⴄ�f�[�^�ێ�������
�ESTL�̓����ƊO���Ɋւ�����������Ȃ�(�@���x�N�g���̏��������Ȃ�)
�E���_���X�g�A���_�̂Ȃ����݂̂ɂ���ăf�[�^�ێ����s��
�E���ۂ̕��̂ŕ\�����\�ȃf�[�^�������Ȃ��Ɖ��肷��(�S�Ă̒��_�ŎO�p�`�̖ʂ��Ȃ���Ă���A�����؂��)
STL�f�[�^�o�͎��ɂ��̐������S�����Ƃ�����(�������)

p : ���_�̍��W
relate : ���_�̌q������B���_�̓Y�����x�[�X�Ōq�������w��
*/
#ifndef STL
#define STL
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <set>
#include "Point.hpp"
using ll=long long; 
using namespace std;

class Stl{
private:
   string name;//STL�t�@�C�����̖��O
   vector<Point> p;//���_���X�g
   vector<tuple<ll,ll,ll>> relate;//�O�p�`�̖ʂ̍���

public:
   Stl();
   void load(string filename);
   void rename(string s);
   void export(string filename);
};

#endif
