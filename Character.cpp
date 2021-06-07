#include "Character.h"
#define HeroBaseState_ID                0   //Ӣ��ID
#define HeroBaseState_Name              1   //Ӣ������
#define HeroBaseState_Level             2   //Ӣ���Ǽ�
#define HeroBaseState_ModelID           3   //Ӣ��ģ��ID
#define HeroBaseState_AD                4   //������
#define HeroBaseState_AP                5   //��ǿ
#define HeroBaseState_ARG               6   //������Χ
#define HeroBaseState_AS                7   //����
#define HeroBaseState_MR                8   //ħ��
#define HeroBaseState_AR                9   //����
#define HeroBaseState_Speed             10  //����
#define HeroBaseState_CRIT              11  //������
#define HeroBaseState_InitMANA          12  //��ʼ����ֵ
#define HeroBaseState_HP                13  //Ѫ��
//��ʼλ��


/**************************************************skill�ຯ��ʵ��**********************************************/
void skill::setSkill(int ad, int ap, int real, int regen, int dbf, int dbfe, int bf,int bfe)
{
	damageAD = ad;
	damageAP = ap;
	damageREAL = real;
	Regen = regen;
	Debuff = dbf;
	DebuffEffect = dbfe;
	Buff = bf;
	BuffEffect = bfe;
}
/**************************************************Hero�ຯ��ʵ��**********************************************/

Hero::Hero()
{
	;
}

Hero::~Hero()
{
	/*����Ӣ�ۺ���䳷��*/;
}

void Hero::bindSprite(Sprite* sprite)
{
	if (this->h_sprite != NULL) {
		h_sprite->removeFromParentAndCleanup(true);
	}
	this->h_sprite = sprite;
	this->addChild(h_sprite);

	Size size = h_sprite->getContentSize();
	this->setContentSize(size);
	onBindSprite();
}

Sprite* Hero::getSprite()
{
	return this->h_sprite;
}

Hero* Hero::create(Sprite* sprite)
{
	Hero* hero = new Hero();
	if (hero && hero->init(sprite)) {
		hero->autorelease();
	}
	else {
		CC_SAFE_DELETE(hero);
	}
	return hero;
}

bool Hero::init(Sprite* sprite)
{
	bool bRet = false;
	do {
		CC_BREAK_IF(!sprite);
		bindSprite(sprite);
		bRet = true;
	} while (0);
	return bRet;
}

//�������ļ��в���ĳ��ID��Ӣ�ۣ���ȡ����Ӣ�۵����ԣ�Ȼ�󴴽�Ӣ�۶���
bool Hero::initFromCsvFileByID(int iHeroID,int level)
{
	bool bRet = false;
	do {
		CsvUtil* csvUtil = CsvUtil::getInstance();
		std::string sHeroID = StringUtils::toString(iHeroID);

		/*Ѱ��ID������*/
		int iLine = csvUtil->findValueInWithLine(sHeroID.c_str(), HeroBaseState_ID, PATH_CSV_HERO_CONF);
		CC_BREAK_IF(iLine < 0);

		setfID(iHeroID);
		setfModelID(csvUtil->getInt(iLine, HeroBaseState_ModelID, PATH_CSV_HERO_CONF));
		setfLevel(csvUtil->getInt(iLine, HeroBaseState_Level, PATH_CSV_HERO_CONF));
		setfHP(csvUtil->getInt(iLine, HeroBaseState_HP, PATH_CSV_HERO_CONF));
		setfAD(csvUtil->getInt(iLine, HeroBaseState_AD, PATH_CSV_HERO_CONF));
		//setiCurAtk(getiBaseAtk());
		setfAP(csvUtil->getInt(iLine, HeroBaseState_AP, PATH_CSV_HERO_CONF));
		setfAS(csvUtil->getInt(iLine, HeroBaseState_AS, PATH_CSV_HERO_CONF));
		setfARG(csvUtil->getInt(iLine, HeroBaseState_ARG, PATH_CSV_HERO_CONF));
		setfAR(csvUtil->getInt(iLine, HeroBaseState_AR, PATH_CSV_HERO_CONF));
		setfMR(csvUtil->getInt(iLine, HeroBaseState_MR, PATH_CSV_HERO_CONF));
		setfSpeed(csvUtil->getInt(iLine, HeroBaseState_Speed, PATH_CSV_HERO_CONF));
		setfCRIT(csvUtil->getInt(iLine, HeroBaseState_CRIT, PATH_CSV_HERO_CONF));
		setfInitMANA(csvUtil->getInt(iLine, HeroBaseState_InitMANA, PATH_CSV_HERO_CONF));

		Sprite* sprite = Sprite::create(StringUtils::format("Ӣ��ͼƬ.png", iHeroID).c_str());
		CC_BREAK_IF(!sprite);
		CC_BREAK_IF(!init(sprite));
		bRet = true;
	} while (0);

	return bRet;
}

//���޸�
void Hero::hurtMe(int iDamage,int dmgType)
{
	if (h_isDead) {
		return;
	}
	int hurt,mana;
	switch (dmgType) {
		case AD_damage:
			hurt = iDamage * (1 - getfAR() / (getfAR() + 100));
			break;
		case AP_damage:
			hurt = iDamage * (1 - getfMR() / (getfMR() + 100));
			break;
		case REAL_damage:
			hurt = iDamage;
			break;
	}
	if (h_iMaxMANA) {
		if (hurt <= 100)
			mana = hurt / 10;
		else if (hurt <= 300)
			mana = hurt / 15;
		else if (hurt <= 600)
			mana = hurt / 20;
		else if (hurt <= 1000)
			mana = hurt / 25;
		else
			mana = hurt / 30;
		if (mana > 50)
			mana = 50;
	}
	if (hurt <= h_Shield) {
		h_Shield -= hurt;
		return;
	}
	hurt -= h_Shield;                       //��ȥ����
	int iCurHP = getfCurHP();               //��ǰHP
	int iAfterHP = iCurHP - hurt;           //�ܻ���HP
	mana += getfCurMANA();
	setfCurMANA(mana);      
	onHurt(hurt,mana);
	if (iAfterHP > 0) {
		setfCurHP(iAfterHP);
	}
	else {
		h_isDead = true;
		/*����*/
		onDead();
	}
}

void Hero::UseSkill()
{
	;
}

bool Hero::isDead()
{
	return this->h_isDead;
}
//���������������
void Hero::onDead() 
{
	h_isTargeted=false;
	/*��������*/;
}
//�󶨾������
void Hero::onBindSprite()
{
	;
}
//�ܻ�
void Hero::onHurt(int iHurtValue,int mana)
{
	/*�ܵ����������Ч��ʾ*/;
}

int Hero::CmnATKDamage(Hero* target)
{
	if (h_isDead) {   //����˲���������˺�������
		return 0;
	}
	int damage;
	damage = getfAD();
	target->hurtMe(damage, AD_damage);
	return 0;
	
}

void Hero::CmnAtkMANA()
{
	int mana;
	mana = getfCurMANA()+h_ATKMANA;
	if (mana >= h_iMaxMANA) {
		setfCurMANA(h_iMaxMANA);
	}
	else {
		setfCurMANA(mana);
	}
	return;
}

/*int Hero::SkillATKDamage(Hero* target)
{
	if (h_isDead) {   //����˲���������˺�������
		return 0;
	}
	int damage1,damage2,damage3,damageALL;
	damage1 = h_Skill.damageAD;
	target->hurtMe(damage1, AD_damage);
	damage2 = h_Skill.damageAP;
	target->hurtMe(damage2, AP_damage);
	damage3 = h_Skill.damageREAL;
	target->hurtMe(damage3, REAL_damage);
	setfCurMANA(0);//�����ͷź���������
}*/


Hero* Hero::createFromCsvFileByID(int iHeroID,int level)
{
	Hero* hero = new Hero();
	if (hero && hero->initFromCsvFileByID(iHeroID,level)) {
		hero->autorelease();
	}
	else {
		CC_SAFE_DELETE(hero);
		return hero;
	}
}

void Hero::wearEqpmt(void (*a)(Hero*))
{
	a(this);
}


/**************************************ManageHero����ʵ��********************************************/
void Hero::GoOnStage()
{
	h_onStage = true;
	/*������vector�����ϳ�������*/
	Manager.onStageHeroes.pushBack(this);
	/*���map�����ϳ�Ӣ���������*/
	Vector<int>::iterator occupation = h_occupation.begin(), feature = h_feature.begin();
	for (; occupation !=h_occupation.end(); occupation++) {
		Manager.HeroRelation[*occupation]++;
	}
	for (; feature != h_feature.end(); feature++) {
		Manager.HeroRelation[*feature]++;
	}
	return;
}

void Hero::OutOfStage()
{
	/*ɾ������Ӣ������*/
	Vector<Hero*>::iterator h_on_stage = Manager.onStageHeroes.begin();
	for (; h_on_stage != Manager.onStageHeroes.end(); h_on_stage++) {
		if (this == *h_on_stage) {
			Manager.onStageHeroes.erase(h_on_stage);
		}
	}
	/*ɾ������Я�����*/
	Vector<int>::iterator occupation = h_occupation.begin(), feature = h_feature.begin();
	for (; occupation != h_occupation.end(); occupation++) {
		Manager.HeroRelation[*occupation]--;
	}
	for (; feature != h_feature.end(); feature++) {
		Manager.HeroRelation[(*occupation)]--;
	}
	h_onStage = false;
}


/**************************************��Ӣ���ಿ��ʵ��********************************************/
Fuajimier::Fuajimier(int  level)
{
	int ad=0, ap=0, real=0, regen=0, dbf=0,dbfe=0, bf=0,bfe=0;
	h_ID = HERO_Fulajimier;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "�������׶�";
	h_iLevel = level;
	h_iAR = 35;
	h_iMR = 35;
	h_iAS = 0.65;
	h_iCRIT = 0.25;
	h_iARG = 2;
	h_iInitMANA = 0;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 85;
	h_iCost = 1;
	h_feature.pushBack(Feature_HEIYE);
	h_occupation.pushBack(Occupation_FUSUZHE);
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 650;
			h_iAD = 45;
			h_iSell = 1;
			ap = 300;
			regen = 200;
			break;
		case 2:
			h_iMaxHP = 1170;
			h_iAD = 81;
			h_iSell = 3;
			ap = 420;
			regen = 250;
			break;
		case 3:
			h_iMaxHP = 2106;
			h_iAD = 146;
			h_iSell = 9;
			ap = 540;
			regen =350;
			break;
	}
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf, dbfe,bf,bfe);
	/*���ı���ȡ����˵��*/
	
}

void Fuajimier::UseSkill()
{
	if (h_isDead) {   //����˲���������˺�������
		return ;
	}
	Target->hurtMe(h_Skill.damageAD, AD_damage);

	Target->hurtMe(h_Skill.damageAP, AP_damage);
	
	Target->hurtMe(h_Skill.damageREAL, REAL_damage);
	setfCurMANA(0);//�����ͷź���������
	/*��Ѫ*/
	int hp = h_iCurHP;
	hp += h_Skill.Regen;
	if (hp >= h_iMaxHP) {
		setfCurHP(h_iMaxHP);
	}
	else {
		setfCurHP(hp);
	}
	/*��Ч*/
}

Wolike::Wolike(int level)
{
	int ad = 0, ap = 0, real = 0, regen = 0, dbf = Debuff_stun, dbfe = 2, bf = 0, bfe = 0;
	h_ID = HERO_Wolike;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "�����";
	h_iLevel = level;
	h_iAR = 40;
	h_iMR = 40;
	h_iAS = 0.6;
	h_iCRIT = 0.25;
	h_iARG = 1;
	h_iInitMANA = 60;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 100;
	h_iCost = 1;
	h_feature.pushBack(Feature_POBAI);
	h_occupation.pushBack(Occupation_DOUSHI);
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 700;
			h_iAD = 55;
			h_iSell = 1;
			ap = 200;
			break;
		case 2:
			h_iMaxHP = 1260;
			h_iAD = 99;
			h_iSell = 3;
			ap = 300;
			break;
		case 3:
			h_iMaxHP = 2268;
			h_iAD = 178;
			h_iSell = 9;
			ap = 450;
			break;
	}
	regen = ap * 0.8;
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf,dbfe, bf,bfe);
	/*���ı���ȡ����˵��*/
}

void Wolike::UseSkill()
{
	if (h_isDead) {   //����˲���������˺�������
		return;
	}
	Target->hurtMe(h_Skill.damageAD, AD_damage);

	Target->hurtMe(h_Skill.damageAP, AP_damage);

	Target->hurtMe(h_Skill.damageREAL, REAL_damage);
	setfCurMANA(0);//�����ͷź���������
	/*ѣ��*/
	Target->h_Debuff = h_Skill.Debuff;
	Target->h_DebuffEffect = h_Skill.DebuffEffect;
	/*��Ѫ*/
	int hp = h_iCurHP;
	hp += h_Skill.Regen;
	if (hp >= h_iMaxHP) {
		setfCurHP(h_iMaxHP);
	}
	else {
		setfCurHP(hp);
	}
	/*��Ч*/
}

VN::VN(int level)
{
	int ad = 0, ap = 0, real = 0, regen = 0, dbf = 0, dbfe = 0, bf = 0, bfe = 0;
	h_ID = HERO_VN;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "ޱ��";
	h_iLevel = level;
	h_iAR = 15;
	h_iMR = 15;
	h_iAS = 0.8;
	h_iCRIT = 0.25;
	h_iARG = 4;
	h_iInitMANA = 0;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 0;
	h_iCost = 1;
	h_feature.pushBack(Feature_POBAI);
	h_occupation.pushBack(Occupation_YOUXIA);
	h_ATKMANA = 0;
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 500;
			h_iAD = 30;
			h_iSell = 1;
			real = 65;
			break;
		case 2:
			h_iMaxHP = 900;
			h_iAD = 54;
			h_iSell = 3;
			real = 90;
			break;
		case 3:
			h_iMaxHP = 1620;
			h_iAD = 97;
			h_iSell = 9;
			real = 140;
			break;
	}
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf, dbfe, bf, bfe);
	/*���ı���ȡ����˵��*/
}

void VN::UseSkill()
{
	if (h_isDead) {   //����˲���������˺�������
		return;
	}
	
	/*û�����ôд*/

	/*��Ч*/
}

Bobi::Bobi(int level)
{
	int ad = 0, ap = 0, real = 0, regen = 0, dbf = 0, dbfe = 0, bf = 0, bfe = 0;
	h_ID = HERO_Bobi;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "����";
	h_iLevel = level;
	h_iAR = 40;
	h_iMR = 40;
	h_iAS = 0.55;
	h_iCRIT = 0.25;
	h_iARG = 1;
	h_iInitMANA = 40;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 80;
	h_iCost = 1;
	h_feature.pushBack(Feature_XIAOEMO);
	h_occupation.pushBack(Occupation_QISHI);
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 600;
			h_iAD = 80;
			h_iSell = 1;
			ap = 150;
			bf = Buff_shield;
			bfe = 250;
			break;
		case 2:
			h_iMaxHP = 1080;
			h_iAD = 144;
			h_iSell = 3;
			ap = 250;
			bf = Buff_shield;
			bfe = 350;
			break;
		case 3:
			h_iMaxHP = 1944;
			h_iAD = 259;
			h_iSell = 9;
			ap = 350;
			bf = Buff_shield;
			bfe = 450;
			break;
	}
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf, dbfe, bf, bfe);
	/*���ı���ȡ����˵��*/
}

void Bobi::UseSkill()
{
	if (h_isDead) {   //����˲���������˺�������
		return;
	}
	Target->hurtMe(h_Skill.damageAD, AD_damage);

	Target->hurtMe(h_Skill.damageAP, AP_damage);

	Target->hurtMe(h_Skill.damageREAL, REAL_damage);
	/*����*/
	h_Shield = h_Skill.BuffEffect;
	setfCurMANA(0);//�����ͷź���������
	
	/*��Ч*/
}

UD::UD(int  level)
{
	int ad = 0, ap = 0, real = 0, regen = 0, dbf = 0, dbfe = 0, bf = 0, bfe = 0;
	h_ID = HERO_UD;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "�ڵ϶�";
	h_iLevel = level;
	h_iAR = 30;
	h_iMR = 30;
	h_iAS = 0.75;
	h_iCRIT = 0.25;
	h_iARG = 1;
	h_iInitMANA = 30;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 40;
	h_iCost = 1;
	h_feature.pushBack(Feature_LONGZU);
	h_occupation.pushBack(Occupation_SHENDUNZHANSHI);
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 700;
			h_iAD = 55;
			h_iSell = 1;
			break;
		case 2:
			h_iMaxHP = 1260;
			h_iAD = 99;
			h_iSell = 3;
			break;
		case 3:
			h_iMaxHP = 2268;
			h_iAD = 178;
			h_iSell = 9;
			break;
	}
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf, dbfe, bf, bfe);
	/*���ı���ȡ����˵��*/

}

void UD::UseSkill()
{
	if (h_isDead) {   //����˲���������˺�������
		return;
	}
	static int num = 0;
	switch (num % 2) {
		case 0:
			switch (h_iLevel) {
				case 1:
					Target->hurtMe(h_iAD*1.2, AD_damage);
					break;
				case 2:
					Target->hurtMe(h_iAD * 1.3, AD_damage);
					break;
				case 3:
					Target->hurtMe(h_iAD * 1.8, AD_damage);
					break;
			}
			break;
		case 1:
			switch (h_iLevel) {
				case 1:
					h_Shield = 250;
					break;
				case 2:
					h_Shield = 350;
					break;
				case 3:
					h_Shield = 550;
					break;
			}
			break;
	}
	num++;
	setfCurMANA(0);//�����ͷź���������
	/*��Ч*/
}

Gulajiasi::Gulajiasi(int level)
{
	int ad = 0, ap = 0, real = 0, regen = 0, dbf = 0, dbfe = 0, bf = 0, bfe = 0;
	h_ID = HERO_Gulajiasi;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "������˹";
	h_iLevel = level;
	h_iAR = 40;
	h_iMR = 40;
	h_iAS = 0.5;
	h_iCRIT = 0.25;
	h_iARG = 2;
	h_iInitMANA = 0;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 50;
	h_iCost = 1;
	h_feature.pushBack(Feature_LIMING);
	h_occupation.pushBack(Occupation_DOUSHI);
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 750;
			h_iAD = 50;
			h_iSell = 1;
			ap = 175;
			break;
		case 2:
			h_iMaxHP = 1350;
			h_iAD = 90;
			h_iSell = 3;
			ap = 250;
			break;
		case 3:
			h_iMaxHP = 2430;
			h_iAD = 162;
			h_iSell = 9;
			ap = 400;
			break;
	}
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf, dbfe, bf, bfe);
	/*���ı���ȡ����˵��*/
}

void Gulajiasi::UseSkill()
{
	if (h_isDead) {   //����˲���������˺�������
		return;
	}
	Target->hurtMe(h_Skill.damageAD, AD_damage);

	Target->hurtMe(h_Skill.damageAP, AP_damage);

	Target->hurtMe(h_Skill.damageREAL, REAL_damage);
	setfCurMANA(0);//�����ͷź���������

	/*��Ч*/
}

Leiouna::Leiouna(int level)
{
	int ad = 0, ap = 0, real = 0, regen = 0, dbf = 0, dbfe = 0, bf = 0, bfe = 0;
	h_ID = HERO_Leiouna;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "��ŷ��";
	h_iLevel = level;
	h_iAR = 40;
	h_iMR = 40;
	h_iAS = 0.55;
	h_iCRIT = 0.25;
	h_iARG = 1;
	h_iInitMANA = 0;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 60;
	h_iCost = 1;
	h_feature.pushBack(Feature_SHENGGUANGWEISHI);
	h_occupation.pushBack(Occupation_QISHI);
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 700;
			h_iAD = 80;
			h_iSell = 1;
			break;
		case 2:
			h_iMaxHP = 1260;
			h_iAD = 144;
			h_iSell = 3;
			break;
		case 3:
			h_iMaxHP = 2268;
			h_iAD = 259;
			h_iSell = 9;
			break;
	}
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf, dbfe, bf, bfe);
	/*���ı���ȡ����˵��*/
}

void Leiouna::UseSkill()
{
	if (h_isDead) {   //����˲���������˺�������
		return;
	}
	Target->hurtMe(h_Skill.damageAD, AD_damage);

	Target->hurtMe(h_Skill.damageAP, AP_damage);

	Target->hurtMe(h_Skill.damageREAL, REAL_damage);
	setfCurMANA(0);//�����ͷź���������
	/*�˺�����*/
	/*��Ч*/
}

Giggs::Giggs(int level)
{
	int ad = 0, ap = 0, real = 0, regen = 0, dbf = 0, dbfe = 0, bf = 0, bfe = 0;
	h_ID = HERO_Giggs;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "����˹";
	h_iLevel = level;
	h_iAR = 15;
	h_iMR = 15;
	h_iAS = 0.75;
	h_iCRIT = 0.25;
	h_iARG = 4;
	h_iInitMANA = 0;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 40;
	h_iCost = 1;
	h_feature.pushBack(Feature_XIAOEMO);
	h_occupation.pushBack(Occupation_FASHI);
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 500;
			h_iAD = 40;
			h_iSell = 1;
			ap = 250;
			dbf = Debuff_stun;
			dbfe = 1;
			break;
		case 2:
			h_iMaxHP = 900;
			h_iAD = 72;
			h_iSell = 3;
			ap = 350;
			dbf = Debuff_stun;
			dbfe = 1;
			break;
		case 3:
			h_iMaxHP = 1260;
			h_iAD = 130;
			h_iSell = 9;
			ap = 550;
			dbf = Debuff_stun;
			dbfe = 1;
			break;
	}
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf, dbfe, bf, bfe);
	/*���ı���ȡ����˵��*/
}

void Giggs::UseSkill()
{
	if (h_isDead) {   //����˲���������˺�������
		return;
	}
	Target->hurtMe(h_Skill.damageAD, AD_damage);

	Target->hurtMe(h_Skill.damageAP, AP_damage);

	Target->hurtMe(h_Skill.damageREAL, REAL_damage);
	setfCurMANA(0);//�����ͷź���������

	/*��Ч*/
}

Kazike::Kazike(int level)
{
	int ad = 0, ap = 0, real = 0, regen = 0, dbf = 0, dbfe = 0, bf = 0, bfe = 0;
	h_ID = HERO_Kazike;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "���ȿ�";
	h_iLevel = level;
	h_iAR = 25;
	h_iMR = 25;
	h_iAS = 0.7;
	h_iCRIT = 0.25;
	h_iARG = 2;
	h_iInitMANA = 0;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 60;
	h_iCost = 1;
	h_feature.pushBack(Feature_LIMING);
	h_occupation.pushBack(Occupation_CIKE);
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 500;
			h_iAD = 55;
			h_iSell = 1;
			ap = 250;
			break;
		case 2:
			h_iMaxHP = 900;
			h_iAD = 99;
			h_iSell = 3;
			ap = 350;
			break;
		case 3:
			h_iMaxHP = 1620;
			h_iAD = 178;
			h_iSell = 9;
			ap = 550;
			break;
	}
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf, dbfe, bf, bfe);
	/*���ı���ȡ����˵��*/
}

void Kazike::UseSkill()
{
	if (h_isDead) {   //����˲���������˺�������
		return;
	}
	bool yon=false;
	/*�ж���Χ�Ƿ��е���*/
	if (yon) {
		switch (h_iLevel) {
			case 1:
				h_Skill.damageAP = 750;
				break;
			case 2:
				h_Skill.damageAP = 1050;
				break;
			case 3:
				h_Skill.damageAP = 1650;
				break;
		}
	}
	Target->hurtMe(h_Skill.damageAD, AD_damage);

	Target->hurtMe(h_Skill.damageAP, AP_damage);

	Target->hurtMe(h_Skill.damageREAL, REAL_damage);
	setfCurMANA(0);//�����ͷź���������
	/*��Ч*/
}

Lisangzhuo::Lisangzhuo(int level)
{
	int ad = 0, ap = 0, real = 0, regen = 0, dbf = 0, dbfe = 0, bf = 0, bfe = 0;
	h_ID = HERO_Lisangzhuo;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "��ɣ׿";
	h_iLevel = level;
	h_iAR = 20;
	h_iMR = 20;
	h_iAS = 0.65;
	h_iCRIT = 0.25;
	h_iARG = 3;
	h_iInitMANA = 0;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 50;
	h_iCost = 1;
	h_feature.pushBack(Feature_MONV);
	h_occupation.pushBack(Occupation_FUSUZHE);
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 550;
			h_iAD = 40;
			h_iSell = 1;
			ap = 250;
			dbf = Debuff_lowerAD;
			dbfe = 8;
			break;
		case 2:
			h_iMaxHP = 990;
			h_iAD = 72;
			h_iSell = 3;
			ap = 300;
			dbf = Debuff_lowerAD;
			dbfe = 8;
			break;
		case 3:
			h_iMaxHP = 1782;
			h_iAD = 130;
			h_iSell = 9;
			ap = 400;
			dbf = Debuff_lowerAD;
			dbfe = 8;
			break;
	}
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf, dbfe, bf, bfe);
	/*���ı���ȡ����˵��*/
}

void Lisangzhuo::UseSkill()
{
	if (h_isDead) {   //����˲���������˺�������
		return;
	}
	Target->hurtMe(h_Skill.damageAD, AD_damage);

	Target->hurtMe(h_Skill.damageAP, AP_damage);

	Target->hurtMe(h_Skill.damageREAL, REAL_damage);
	
	setfCurMANA(0);//�����ͷź���������
	
	/*��Ч*/
}

Kelie::Kelie(int level)
{
	int ad = 0, ap = 0, real = 0, regen = 0, dbf = 0, dbfe = 0, bf = 0, bfe = 0;
	h_ID = HERO_Lisangzhuo;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "����";
	h_iLevel = level;
	h_iAR = 30;
	h_iMR = 30;
	h_iAS = 0.7;
	h_iCRIT = 0.25;
	h_iARG = 1;
	h_iInitMANA = 0;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 0;
	h_iCost = 1;
	h_feature.pushBack(Feature_XIAOEMO);
	h_occupation.pushBack(Occupation_ZHONGQIBING);
	h_ATKMANA = 0;
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 400;
			h_iAD = 60;
			h_iSell = 1;
			ap = 250;
			break;
		case 2:
			h_iMaxHP = 720;
			h_iAD = 108;
			h_iSell = 3;
			ap = 300;
			break;
		case 3:
			h_iMaxHP = 1296;
			h_iAD = 194;
			h_iSell = 9;
			ap = 400;
			break;
	}
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf, dbfe, bf, bfe);
	/*���ı���ȡ����˵��*/
}

void Kelie::UseSkill()
{
	if (h_isDead) {   //����˲���������˺�������
		return;
	}
	h_Shield = h_iMaxHP * 0.8;


	/*��Ч*/
}

Yatuokesi::Yatuokesi(int level)
{
	int ad = 0, ap = 0, real = 0, regen = 0, dbf = 0, dbfe = 0, bf = 0, bfe = 0;
	h_ID = HERO_Yatuokesi;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "���п�˹";
	h_iLevel = level;
	h_iAR = 30;
	h_iMR = 30;
	h_iAS = 0.65;
	h_iCRIT = 0.25;
	h_iARG = 2;
	h_iInitMANA = 0;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 30;
	h_iCost = 1;
	h_feature.pushBack(Feature_SHENGGUANGWEISHI);
	h_occupation.pushBack(Occupation_ZHENGFUZHE);
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 650;
			h_iAD = 60;
			h_iSell = 1;
			ad = h_iAD*2.6;
			regen = 200;
			break;
		case 2:
			h_iMaxHP = 1170;
			h_iAD = 108;
			h_iSell = 3;
			ad = h_iAD * 2.8;
			regen = 300;
			break;
		case 3:
			h_iMaxHP = 2106;
			h_iAD = 194.4;
			h_iSell = 9;
			ad = h_iAD * 3.6;
			regen = 450;
			break;
	}
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf, dbfe, bf, bfe);
	/*���ı���ȡ����˵��*/
}

void Yatuokesi::UseSkill()
{
	if (h_isDead) {   //����˲���������˺�������
		return;
	}
	Target->hurtMe(h_Skill.damageAD, AD_damage);

	Target->hurtMe(h_Skill.damageAP, AP_damage);

	Target->hurtMe(h_Skill.damageREAL, REAL_damage);
	setfCurMANA(0);//�����ͷź���������
	/*��Ѫ*/
	int hp = h_iCurHP;
	hp += h_Skill.Regen;
	if (hp >= h_iMaxHP) {
		setfCurHP(h_iMaxHP);
	}
	else {
		setfCurHP(hp);
	}
	/*��Ч*/
}


Kalisita::Kalisita(int level)
{
	int ad = 0, ap = 0, real = 0, regen = 0, dbf = 0, dbfe = 0, bf = 0, bfe = 0;
	h_ID = HERO_Fulajimier;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "����˹��";
	h_iLevel = level;
	h_iAR = 15;
	h_iMR = 15;
	h_iAS = 0.75;
	h_iCRIT = 0.25;
	h_iARG = 4;
	h_iInitMANA = 0;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 120;
	h_iCost = 1;
	h_feature.pushBack(Feature_SANGSHI);
	h_occupation.pushBack(Occupation_ZHENGFUZHE);
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 500;
			h_iAD = 60;
			h_iSell = 1;
			ap = h_iAD*1.8;
			ad = 350;
			break;
		case 2:
			h_iMaxHP = 900;
			h_iAD = 108;
			h_iSell = 3;
			ap = h_iAD*2;
			ad = 600;
			break;
		case 3:
			h_iMaxHP = 1620;
			h_iAD = 194;
			h_iSell = 9;
			ap = h_iAD*2.4;
			ad = 600;
			break;
	}
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf, dbfe, bf, bfe);
	/*���ı���ȡ����˵��*/
}

void Kalisita::UseSkill()
{
	if (h_isDead) {   //����˲���������˺�������
		return;
	}
	Target->hurtMe(h_Skill.damageAD, AD_damage);

	Target->hurtMe(h_Skill.damageAP, AP_damage);

	Target->hurtMe(h_Skill.damageREAL, REAL_damage);
	setfCurMANA(0);//�����ͷź���������
	
	/*��Ч*/
}

Lefulan::Lefulan(int level)
{
	int ad = 0, ap = 0, real = 0, regen = 0, dbf = 0, dbfe = 0, bf = 0, bfe = 0;
	h_ID = HERO_Lefulan;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "��ܽ��";
	h_iLevel = level;
	h_iAR = 20;
	h_iMR = 20;
	h_iAS = 0.75;
	h_iCRIT = 0.25;
	h_iARG = 3;
	h_iInitMANA = 20;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 60;
	h_iCost = 2;
	h_feature.pushBack(Feature_MONV);
	h_occupation.pushBack(Occupation_CIKE);
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 550;
			h_iAD = 55;
			h_iSell = 2;
			ap = 300;
			regen = 200;
			dbf = Debuff_stun;
			dbfe = 1;
			break;
		case 2:
			h_iMaxHP = 990;
			h_iAD = 99;
			h_iSell = 5;
			ap = 420;
			regen = 250;
			dbf = Debuff_stun;
			dbfe = 2;
			break;
		case 3:
			h_iMaxHP = 1782;
			h_iAD = 178;
			h_iSell = 14;
			ap = 540;
			regen = 350;
			dbf = Debuff_stun;
			dbfe = 3;
			break;
	}
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf, dbfe, bf, bfe);
	/*���ı���ȡ����˵��*/
}

void Lefulan::UseSkill()
{
	if (h_isDead) {   //����˲���������˺�������
		return;
	}
	Target->hurtMe(h_Skill.damageAD, AD_damage);

	Target->hurtMe(h_Skill.damageAP, AP_damage);

	Target->hurtMe(h_Skill.damageREAL, REAL_damage);
	setfCurMANA(0);//�����ͷź���������
	/*ѣ��*/
	Sleep(1000);
	Target->h_Debuff = h_Skill.Debuff;
	Target->h_DebuffEffect = h_Skill.DebuffEffect;
	/*��������ʱ��*/
	Sleep(500);
	/*��Ч*/
}

Suolaka::Suolaka(int level)
{
	int ad = 0, ap = 0, real = 0, regen = 0, dbf = 0, dbfe = 0, bf = 0, bfe = 0;
	h_ID = HERO_Suolaka;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "������";
	h_iLevel = level;
	h_iAR = 25;
	h_iMR = 25;
	h_iAS = 0.6;
	h_iCRIT = 0.25;
	h_iARG = 3;
	h_iInitMANA = 30;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 70;
	h_iCost = 2;
	h_feature.pushBack(Feature_LIMING);
	h_occupation.pushBack(Occupation_FUSUZHE);
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 550;
			h_iAD = 40;
			h_iSell = 2;
			ap = 150;
			dbfe = 33;
			break;
		case 2:
			h_iMaxHP = 990;
			h_iAD = 72;
			h_iSell = 5;
			ap = 225;
			dbfe = 35;
			break;
		case 3:
			h_iMaxHP = 1782;
			h_iAD = 130;
			h_iSell = 14;
			ap = 350;
			dbfe = 50;
			break;
	}
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf, dbfe, bf, bfe);
	/*���ı���ȡ����˵��*/
}

void Suolaka::UseSkill()
{
	if (h_isDead) {   //����˲���������˺�������
		return;
	}
	Target->hurtMe(h_Skill.damageAD, AD_damage);

	Target->hurtMe(h_Skill.damageAP, AP_damage);

	Target->hurtMe(h_Skill.damageREAL, REAL_damage);

	/*ƽ��*/
	int Cmana,Amana;
	Cmana = Target->getfCurMANA();
	Amana = Cmana - (h_iMaxMANA * double(h_Skill.DebuffEffect)/100);
	if (Amana <= 0)
		Amana = 0;
	Target->setfCurMANA(Amana);
	setfCurMANA(0);//�����ͷź���������
	/*��Ч*/
}

Bulande::Bulande(int level)
{
	int ad = 0, ap = 0, real = 0, regen = 0, dbf = 0, dbfe = 0, bf = 0, bfe = 0;
	h_ID = HERO_Bulande;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "������";
	h_iLevel = level;
	h_iAR = 20;
	h_iMR = 20;
	h_iAS = 0.65;
	h_iCRIT = 0.25;
	h_iARG = 3;
	h_iInitMANA = 0;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 20;
	h_iCost = 2;
	h_feature.pushBack(Feature_SANGSHI);
	h_occupation.pushBack(Occupation_FASHI);
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 550;
			h_iAD = 45;
			h_iSell = 2;
			ap = 300;
			dbfe = 40;
			break;
		case 2:
			h_iMaxHP = 990;
			h_iAD = 81;
			h_iSell = 5;
			ap = 450;
			dbfe = 50;
			break;
		case 3:
			h_iMaxHP = 1782;
			h_iAD = 146;
			h_iSell = 14;
			ap = 650;
			dbfe = 70;
			break;
	}
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf, dbfe, bf, bfe);
	/*���ı���ȡ����˵��*/
}

void Bulande::UseSkill()
{
	if (h_isDead) {   //����˲���������˺�������
		return;
	}
	Target->hurtMe(h_Skill.damageAD, AD_damage);

	Target->hurtMe(h_Skill.damageAP, AP_damage);

	Target->hurtMe(h_Skill.damageREAL, REAL_damage);

	/*����ħ��*/
	int mr = Target->getfMR();
	mr=mr* (100 - h_Skill.DebuffEffect) / 100;
	Target->setfMR(mr);
	setfCurMANA(0);//�����ͷź���������
	/*��Ч*/
}

Kainan::Kainan(int level)
{
	int ad = 0, ap = 0, real = 0, regen = 0, dbf = 0, dbfe = 0, bf = 0, bfe = 0;
	h_ID = HERO_Kainan;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "����";
	h_iLevel = level;
	h_iAR = 30;
	h_iMR = 30;
	h_iAS = 0.65;
	h_iCRIT = 0.25;
	h_iARG = 2;
	h_iInitMANA = 40;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 150;
	h_iCost = 2;
	h_feature.pushBack(Feature_XIAOEMO);
	h_occupation.pushBack(Occupation_SHENDUNZHANSHI);
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 650;
			h_iAD = 50;
			h_iSell = 2;
			ap = 150;
			dbf = Debuff_stun;
			dbfe = 1;
			break;
		case 2:
			h_iMaxHP = 1170;
			h_iAD = 90;
			h_iSell = 5;
			ap = 200;
			dbf = Debuff_stun;
			dbfe = 2;
			break;
		case 3:
			h_iMaxHP = 2106;
			h_iAD = 162;
			h_iSell = 14;
			ap = 540;
			dbf = Debuff_stun;
			dbfe = 3;
			break;
	}
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf, dbfe, bf, bfe);
	/*���ı���ȡ����˵��*/
}

void Kainan::UseSkill()
{
	if (h_isDead) {   //����˲���������˺�������
		return;
	}
	Target->hurtMe(h_Skill.damageAD, AD_damage);

	Target->hurtMe(h_Skill.damageAP, AP_damage);

	Target->hurtMe(h_Skill.damageREAL, REAL_damage);

	/*ѣ��*/
	Target->h_Debuff = Debuff_stun;
	Target->h_DebuffEffect = h_Skill.DebuffEffect;
	setfCurMANA(0);//�����ͷź���������
	/*��Ч*/
}

Weilusi::Weilusi(int level)
{
	int ad = 0, ap = 0, real = 0, regen = 0, dbf = 0, dbfe = 0, bf = 0, bfe = 0;
	h_ID = HERO_Weilusi;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "ά³˹";
	h_iLevel = level;
	h_iAR = 20;
	h_iMR = 20;
	h_iAS = 0.8;
	h_iCRIT = 0.25;
	h_iARG = 4;
	h_iInitMANA = 20;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 75;
	h_iCost = 2;
	h_feature.pushBack(Feature_XIAOEMO);
	h_occupation.pushBack(Occupation_SHENDUNZHANSHI);
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 550;
			h_iAD = 50;
			h_iSell = 2;
			ap = 100;
			ad = h_iAD * 1.5;
			break;
		case 2:
			h_iMaxHP = 990;
			h_iAD = 90;
			h_iSell = 5;
			ap = 150;
			ad = h_iAD * 1.6;
			break;
		case 3:
			h_iMaxHP = 1782;
			h_iAD = 162;
			h_iSell = 14;
			ap = 200;
			ad = h_iAD * 1.5;
			break;
	}
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf, dbfe, bf, bfe);
	/*���ı���ȡ����˵��*/
}

void Weilusi::UseSkill()
{
	if (h_isDead) {   //����˲���������˺�������
		return;
	}
	Target->hurtMe(h_Skill.damageAD, AD_damage);

	Target->hurtMe(h_Skill.damageAP, AP_damage);

	Target->hurtMe(h_Skill.damageREAL, REAL_damage);

	/*ѣ��*/
	Target->h_Debuff = Debuff_stun;
	Target->h_DebuffEffect = h_Skill.DebuffEffect;
	setfCurMANA(0);//�����ͷź���������
	/*��Ч*/
}

Nuotilesi::Nuotilesi(int level)
{
	int ad = 0, ap = 0, real = 0, regen = 0, dbf = 0, dbfe = 0, bf = 0, bfe = 0;
	h_ID = HERO_Nuotilesi;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "ŵ����˹";
	h_iLevel = level;
	h_iAR = 45;
	h_iMR = 45;
	h_iAS = 0.55;
	h_iCRIT = 0.25;
	h_iARG = 5;
	h_iInitMANA = 50;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 120;
	h_iCost = 2;
	h_feature.pushBack(Feature_TIEJIAWEISHI);
	h_occupation.pushBack(Occupation_QISHI);
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 750;
			h_iAD = 50;
			h_iSell = 2;
			ap = 150;
			dbf = Debuff_stun;
			dbfe = 3;
			break;
		case 2:
			h_iMaxHP = 1350;
			h_iAD = 90;
			h_iSell = 5;
			ap = 250;
			dbf = Debuff_stun;
			dbfe = 4;
			break;
		case 3:
			h_iMaxHP = 2430;
			h_iAD = 162;
			h_iSell = 14;
			ap = 700;
			dbf = Debuff_stun;
			dbfe = 6;
			break;
	}
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf, dbfe, bf, bfe);
	/*���ı���ȡ����˵��*/
}

void Nuotilesi::UseSkill()
{
	if (h_isDead) {   //����˲���������˺�������
		return;
	}
	Target->hurtMe(h_Skill.damageAD, AD_damage);

	Target->hurtMe(h_Skill.damageAP, AP_damage);

	Target->hurtMe(h_Skill.damageREAL, REAL_damage);

	/*ѣ��*/
	Target->h_Debuff = h_Skill.Debuff;
	Target->h_DebuffEffect = h_Skill.DebuffEffect;
	setfCurMANA(0);//�����ͷź���������
	/*��Ч*/
}

Weiketuo::Weiketuo(int level)
{
	int ad = 0, ap = 0, real = 0, regen = 0, dbf = 0, dbfe = 0, bf = 0, bfe = 0;
	h_ID = HERO_Weiketuo;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "ά����";
	h_iLevel = level;
	h_iAR = 20;
	h_iMR = 20;
	h_iAS = 0.65;
	h_iCRIT = 0.25;
	h_iARG = 4;
	h_iInitMANA = 30;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 50;
	h_iCost = 2;
	h_feature.pushBack(Feature_POBAI);
	h_occupation.pushBack(Occupation_FASHI);
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 500;
			h_iAD = 45;
			h_iSell = 2;
			ap = 300;
			bf = Buff_shield;
			bfe = 150;
			break;
		case 2:
			h_iMaxHP = 900;
			h_iAD = 81;
			h_iSell = 5;
			ap = 500;
			bf = Buff_shield;
			bfe = 250;
			break;
		case 3:
			h_iMaxHP = 1620;
			h_iAD = 146;
			h_iSell = 14;
			ap = 850;
			bf = Buff_shield;
			bfe = 425;
			break;
	}
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf, dbfe, bf, bfe);
	/*���ı���ȡ����˵��*/
}

void Weiketuo::UseSkill()
{
	if (h_isDead) {   //����˲���������˺�������
		return;
	}
	Target->hurtMe(h_Skill.damageAD, AD_damage);

	Target->hurtMe(h_Skill.damageAP, AP_damage);

	Target->hurtMe(h_Skill.damageREAL, REAL_damage);

	/*����*/
	h_Shield = h_Skill.BuffEffect;
	setfCurMANA(0);//�����ͷź���������
	/*��Ч*/
}

Zhuangseni::Zhuangseni(int  level)
{
	int ad = 0, ap = 0, real = 0, regen = 0, dbf = 0, dbfe = 0, bf = 0, bfe = 0;
	h_ID = HERO_Zhuangseni;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "ׯɪ��";
	h_iLevel = level;
	h_iAR = 45;
	h_iMR = 45;
	h_iAS = 0.55;
	h_iCRIT = 0.25;
	h_iARG = 1;
	h_iInitMANA = 0;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 60;
	h_iCost = 2;
	h_feature.pushBack(Feature_HEIYE);
	h_occupation.pushBack(Occupation_ZHONGQIBING);
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 750;
			h_iAD = 45;
			h_iSell = 2;
			ap = 300;
			dbf = Debuff_stun;
			dbfe = 2;
			bfe = 100;
			break;
		case 2:
			h_iMaxHP = 1350;
			h_iAD = 81;
			h_iSell = 5;
			ap = 450;
			dbf = Debuff_stun;
			dbfe = 3;
			bfe = 150;
			break;
		case 3:
			h_iMaxHP = 2430;
			h_iAD = 146;
			h_iSell = 14;
			ap = 750;
			dbf = Debuff_stun;
			dbfe = 4;
			bfe = 300;
			break;
	}
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf, dbfe, bf, bfe);
	/*���ı���ȡ����˵��*/
}

void Zhuangseni::UseSkill()
{
	if (h_isDead) {   //����˲���������˺�������
		return;
	}
	Target->hurtMe(h_Skill.damageAD, AD_damage);

	Target->hurtMe(h_Skill.damageAP, AP_damage);

	Target->hurtMe(h_Skill.damageREAL, REAL_damage);

	/*������ǿ*/
	h_Shield = h_Skill.BuffEffect;
	h_iAR += h_Skill.BuffEffect;
	h_iMR += h_Skill.BuffEffect;
	setfCurMANA(0);//�����ͷź���������
	/*��Ч*/
}

Hekalimu::Hekalimu(int level)
{
	int ad = 0, ap = 0, real = 0, regen = 0, dbf = 0, dbfe = 0, bf = 0, bfe = 0;
	h_ID = HERO_Hekalimu;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "�տ���ķ";
	h_iLevel = level;
	h_iAR = 45;
	h_iMR = 45;
	h_iAS = 0.55;
	h_iCRIT = 0.25;
	h_iARG = 1;
	h_iInitMANA = 40;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 90;
	h_iCost = 2;
	h_feature.pushBack(Feature_POBAI);
	h_occupation.pushBack(Occupation_ZHONGQIBING);
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 750;
			h_iAD = 55;
			h_iSell = 2;
			ap = 350;
			regen = 300;
			break;
		case 2:
			h_iMaxHP = 1350;
			h_iAD = 99;
			h_iSell = 5;
			ap = 500;
			regen = 400;
			break;
		case 3:
			h_iMaxHP = 2430;
			h_iAD = 178;
			h_iSell = 14;
			ap = 700;
			regen = 600;
			break;
	}
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf, dbfe, bf, bfe);
	/*���ı���ȡ����˵��*/
}

void Hekalimu::UseSkill()
{
	if (h_isDead) {   //����˲���������˺�������
		return;
	}
	Target->hurtMe(h_Skill.damageAD, AD_damage);

	Target->hurtMe(h_Skill.damageAP, AP_damage);

	Target->hurtMe(h_Skill.damageREAL, REAL_damage);
	setfCurMANA(0);//�����ͷź���������
	/*��Ѫ*/
	int hp = h_iCurHP;
	hp += h_Skill.Regen;
	if (hp >= h_iMaxHP) {
		setfCurHP(h_iMaxHP);
	}
	else {
		setfCurHP(hp);
	}
	/*��Ч*/
}

Xindela::Xindela(int level)
{
	int ad = 0, ap = 0, real = 0, regen = 0, dbf = 0, dbfe = 0, bf = 0, bfe = 0;
	h_ID = HERO_Xindela;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "������";
	h_iLevel = level;
	h_iAR = 20;
	h_iMR = 20;
	h_iAS = 0.65;
	h_iCRIT = 0.25;
	h_iARG = 4;
	h_iInitMANA = 40;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 80;
	h_iCost = 2;
	h_feature.pushBack(Feature_SHENGGUANGWEISHI);
	h_occupation.pushBack(Occupation_SHENYUZHE);
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 550;
			h_iAD = 40;
			h_iSell = 2;
			ap = 250;
			dbf = Debuff_knockup;
			dbfe = 1;
			break;
		case 2:
			h_iMaxHP = 990;
			h_iAD = 72;
			h_iSell = 5;
			ap = 450;
			dbf = Debuff_knockup;
			dbfe = 2;
			break;
		case 3:
			h_iMaxHP = 1782;
			h_iAD = 130;
			h_iSell = 14;
			ap = 750;
			dbf = Debuff_knockup;
			dbfe = 3;
			break;
	}
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf, dbfe, bf, bfe);
	/*���ı���ȡ����˵��*/
}

void Xindela::UseSkill()
{
	if (h_isDead) {   //����˲���������˺�������
		return;
	}
	Target->hurtMe(h_Skill.damageAD, AD_damage);

	Target->hurtMe(h_Skill.damageAP, AP_damage);

	Target->hurtMe(h_Skill.damageREAL, REAL_damage);
	
	if (!(Target->h_isDebuff))
		return;
	/*ѣ��*/
	Target->h_Debuff = h_Skill.Debuff;
	Target->h_DebuffEffect = h_Skill.DebuffEffect;
	setfCurMANA(0);//�����ͷź���������

	/*��Ч*/
}

Chuishi::Chuishi(int level)
{
	int ad = 0, ap = 0, real = 0, regen = 0, dbf = 0, dbfe = 0, bf = 0, bfe = 0;
	h_ID = HERO_Chuishi;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "��ʯ";
	h_iLevel = level;
	h_iAR = 40;
	h_iMR = 40;
	h_iAS = 0.6;
	h_iCRIT = 0.25;
	h_iARG = 2;
	h_iInitMANA = 60;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 100;
	h_iCost = 2;
	h_feature.pushBack(Feature_POBAI);
	h_occupation.pushBack(Occupation_QISHI);
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 750;
			h_iAD = 55;
			h_iSell = 2;
			ap = 200;
			dbf = Debuff_stun;
			dbfe = 1;
			break;
		case 2:
			h_iMaxHP = 1350;
			h_iAD = 99;
			h_iSell = 5;
			ap = 400;
			dbf = Debuff_stun;
			dbfe = 2;
			break;
		case 3:
			h_iMaxHP = 2430;
			h_iAD = 178;
			h_iSell = 14;
			ap = 1000;
			dbf = Debuff_stun;
			dbfe = 3;
			break;
	}
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf, dbfe, bf, bfe);
	/*���ı���ȡ����˵��*/
}

void Chuishi::UseSkill()
{
	if (h_isDead) {   //����˲���������˺�������
		return;
	}
	Target->hurtMe(h_Skill.damageAD, AD_damage);

	Target->hurtMe(h_Skill.damageAP, AP_damage);

	Target->hurtMe(h_Skill.damageREAL, REAL_damage);

	if (!(Target->h_isDebuff))
		return;
	/*ѣ��*/
	Target->h_Debuff = h_Skill.Debuff;
	Target->h_DebuffEffect = h_Skill.DebuffEffect;
	setfCurMANA(0);//�����ͷź���������

	/*��Ч*/
}

Seti::Seti(int level)
{
	int ad = 0, ap = 0, real = 0, regen = 0, dbf = 0, dbfe = 0, bf = 0, bfe = 0;
	h_ID = HERO_Seti;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "ɪ��";
	h_iLevel = level;
	h_iAR = 35;
	h_iMR = 35;
	h_iAS = 0.7;
	h_iCRIT = 0.25;
	h_iARG = 1;
	h_iInitMANA = 0;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 40;
	h_iCost = 2;
	h_feature.pushBack(Feature_LONGZU);
	h_occupation.pushBack(Occupation_DOUSHI);
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 700;
			h_iAD = 60;
			h_iSell = 2;
			ad = 160;
			dbfe = 20;
			break;
		case 2:
			h_iMaxHP = 1260;
			h_iAD = 108;
			h_iSell = 5;
			ad = 180;
			dbfe = 25;
			break;
		case 3:
			h_iMaxHP = 2268;
			h_iAD = 194;
			h_iSell = 14;
			ad = 200;
			dbfe = 40;
			break;
	}
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf, dbfe, bf, bfe);
	/*���ı���ȡ����˵��*/
}

void Seti::UseSkill()
{
	h_Skill.damageAD = h_Skill.damageAD *h_iAD / 100;
	if (h_isDead) {   //����˲���������˺�������
		return;
	}
	Target->hurtMe(h_Skill.damageAD, AD_damage);

	Target->hurtMe(h_Skill.damageAP, AP_damage);

	Target->hurtMe(h_Skill.damageREAL, REAL_damage);

	int ar = Target->getfAR();
	ar -= h_Skill.DebuffEffect;
	Target->setfAR(ar);
	setfCurMANA(0);//�����ͷź���������
}

Nunu::Nunu(int level)
{
	int ad = 0, ap = 0, real = 0, regen = 0, dbf = 0, dbfe = 0, bf = 0, bfe = 0;
	h_ID = HERO_Nunu;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "ŬŬ";
	h_iLevel = level;
	h_iAR = 45;
	h_iMR = 45;
	h_iAS = 0.55;
	h_iCRIT = 0.25;
	h_iARG = 1;
	h_iInitMANA = 0;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 70;
	h_iCost = 3;
	h_feature.pushBack(Feature_SANGSHI);
	h_occupation.pushBack(Occupation_DOUSHI);
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 800;
			h_iAD = 65;
			h_iSell = 3;
			ap = 500;
			break;
		case 2:
			h_iMaxHP = 1400;
			h_iAD = 117;
			h_iSell = 8;
			ap = 750;
			break;
		case 3:
			h_iMaxHP = 2592;
			h_iAD = 210;
			h_iSell = 21;
			ap = 1800;
			break;
	}
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf, dbfe, bf, bfe);
	/*���ı���ȡ����˵��*/
}

void Nunu::UseSkill()
{
	if (h_isDead) {   //����˲���������˺�������
		return;
	}
	int chp = Target->getfCurHP();
	if (chp < h_iCurHP){
		h_Skill.damageAP *= 1.5;
		Target->hurtMe(h_Skill.damageAP, REAL_damage);
	}
	Target->hurtMe(h_Skill.damageAD, AD_damage);

	Target->hurtMe(h_Skill.damageAP, AP_damage);

	Target->hurtMe(h_Skill.damageREAL, REAL_damage);

	setfCurMANA(0);//�����ͷź���������  
}

Ashe::Ashe(int level)
{
	int ad = 0, ap = 0, real = 0, regen = 0, dbf = 0, dbfe = 0, bf = 0, bfe = 0;
	h_ID = HERO_Ashe;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "��ϣ";
	h_iLevel = level;
	h_iAR = 20;
	h_iMR = 20;
	h_iAS = 0.7;
	h_iCRIT = 0.25;
	h_iARG = 5;
	h_iInitMANA =50;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 90;
	h_iCost = 3;
	h_feature.pushBack(Feature_SHENYOUZHISEN);
	h_feature.pushBack(Feature_LONGZU);
	h_occupation.pushBack(Occupation_YOUXIA);
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 550;
			h_iAD = 65;
			h_iSell = 3;
			ap = 300;
			dbf = Debuff_stun;
			dbfe = 2;
			break;
		case 2:
			h_iMaxHP = 990;
			h_iAD = 117;
			h_iSell = 8;
			ap = 450;
			dbf = Debuff_stun;
			dbfe = 4;
			break;
		case 3:
			h_iMaxHP = 1782;
			h_iAD = 210;
			h_iSell = 21;
			ap = 600;
			dbf = Debuff_stun;
			dbfe = 7;
			break;
	}
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf, dbfe, bf, bfe);
	/*���ı���ȡ����˵��*/
}

void Ashe::UseSkill()
{
	
	if (h_isDead) {   //����˲���������˺�������
		return;
	}
	Target->hurtMe(h_Skill.damageAD, AD_damage);

	Target->hurtMe(h_Skill.damageAP, AP_damage);

	Target->hurtMe(h_Skill.damageREAL, REAL_damage);
	setfCurMANA(0);//�����ͷź���������
	if (!(Target->h_isDebuff))
		return;
	/*ѣ��*/
	Target->h_Debuff = h_Skill.Debuff;
	Target->h_DebuffEffect = h_Skill.DebuffEffect;
	
}

Moganna::Moganna(int level)
{
	int ad = 0, ap = 0, real = 0, regen = 0, dbf = 0, dbfe = 0, bf = 0, bfe = 0;
	h_ID = HERO_Moganna;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "Ī����";
	h_iLevel = level;
	h_iAR = 30;
	h_iMR = 30;
	h_iAS = 0.6;
	h_iCRIT = 0.25;
	h_iARG = 2;
	h_iInitMANA = 80;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 120;
	h_iCost = 3;
	h_feature.pushBack(Feature_MONV);
	h_feature.pushBack(Feature_HEIYE);
	h_occupation.pushBack(Occupation_MISHUSHI);
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 850;
			h_iAD = 45;
			h_iSell = 3;
			ap = 250;
			real = 150;
			dbf = Debuff_stun;
			dbfe = 2;
			break;
		case 2:
			h_iMaxHP = 1530;
			h_iAD = 81;
			h_iSell = 8;
			ap = 300;
			real = 250;
			dbf = Debuff_stun;
			dbfe = 3;
			break;
		case 3:
			h_iMaxHP = 2754;
			h_iAD = 146;
			h_iSell = 21;
			ap = 500;
			real = 400;
			dbf = Debuff_stun;
			dbfe = 4;
			break;
	}
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf, dbfe, bf, bfe);
	/*���ı���ȡ����˵��*/
}

void Moganna::UseSkill()
{
	if (h_isDead) {   //����˲���������˺�������
		return;
	}
	Target->hurtMe(h_Skill.damageAD, AD_damage);

	Target->hurtMe(h_Skill.damageAP, AP_damage);

	Target->hurtMe(h_Skill.damageREAL, REAL_damage);
	setfCurMANA(0);//�����ͷź���������
	if (!(Target->h_isDebuff))
		return;
	/*ѣ��*/
	Target->h_Debuff = h_Skill.Debuff;
	Target->h_DebuffEffect = h_Skill.DebuffEffect;
	
}

Katelinna::Katelinna(int level)
{
	int ad = 0, ap = 0, real = 0, regen = 0, dbf = 0, dbfe = 0, bf = 0, bfe = 0;
	h_ID = HERO_Katelinna;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "��������";
	h_iLevel = level;
	h_iAR = 30;
	h_iMR = 30;
	h_iAS = 0.8;
	h_iCRIT = 0.25;
	h_iARG = 1;
	h_iInitMANA = 0;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 20;
	h_iCost = 3;
	h_feature.pushBack(Feature_POBAI);
	h_occupation.pushBack(Occupation_CIKE);
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 700;
			h_iAD = 45;
			h_iSell = 3;
			ap = 200;
			break;
		case 2:
			h_iMaxHP = 1260;
			h_iAD = 81;
			h_iSell = 8;
			ap = 250;
			break;
		case 3:
			h_iMaxHP = 2268;
			h_iAD = 146;
			h_iSell = 21;
			ap = 450;
			break;
	}
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf, dbfe, bf, bfe);
	/*���ı���ȡ����˵��*/
}

void Katelinna::UseSkill()
{
	if (h_isDead) {   //����˲���������˺�������
		return;
	}
	Target->hurtMe(h_Skill.damageAD, AD_damage);

	Target->hurtMe(h_Skill.damageAP, AP_damage);

	Target->hurtMe(h_Skill.damageREAL, REAL_damage);
	//նɱ
	int chp = Target->getfCurHP(), mhp = Target->getfMaxHP();
	if (chp <= mhp * 0.05) {
		Target->setfCurHP(0);
	}
	setfCurMANA(0);//�����ͷź���������
}

NOC::NOC(int level)
{
	int ad = 0, ap = 0, real = 0, regen = 0, dbf = 0, dbfe = 0, bf = 0, bfe = 0;
	h_ID = HERO_NOC;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "ħ��";
	h_iLevel = level;
	h_iAR = 30;
	h_iMR = 30;
	h_iAS = 0.85;
	h_iCRIT = 0.25;
	h_iARG = 1;
	h_iInitMANA = 0;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 60;
	h_iCost = 3;
	h_feature.pushBack(Feature_FUSHENGWANGHUN);
	h_occupation.pushBack(Occupation_CIKE);
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 600;
			h_iAD = 80;
			h_iSell = 3;
			ad = 350;
			regen = h_iMaxHP * 0.1;
			break;
		case 2:
			h_iMaxHP = 1080;
			h_iAD = 144;
			h_iSell = 8;
			ad = 500;
			regen = h_iMaxHP * 0.15;
			break;
		case 3:
			h_iMaxHP = 1944;
			h_iAD = 259;
			h_iSell = 21;
			ad = 750;
			regen = h_iMaxHP * 0.2;
			break;
	}
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf, dbfe, bf, bfe);
	/*���ı���ȡ����˵��*/
}

void NOC::UseSkill()
{
	if (h_isDead) {   //����˲���������˺�������
		return;
	}
	Target->hurtMe(h_Skill.damageAD, AD_damage);

	Target->hurtMe(h_Skill.damageAP, AP_damage);

	Target->hurtMe(h_Skill.damageREAL, REAL_damage);
	/*��Ѫ*/
	int hp = h_iCurHP;
	hp += h_Skill.Regen;
	if (hp >= h_iMaxHP) {
		setfCurHP(h_iMaxHP);
	}
	else {
		setfCurHP(hp);
	}
	setfCurMANA(0);//�����ͷź���������
}

Lee::Lee(int level)
{
	int ad = 0, ap = 0, real = 0, regen = 0, dbf = 0, dbfe = 0, bf = 0, bfe = 0;
	h_ID = HERO_Lee;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "����";
	h_iLevel = level;
	h_iAR = 35;
	h_iMR = 35;
	h_iAS = 0.75;
	h_iCRIT = 0.25;
	h_iARG = 1;
	h_iInitMANA = 30;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 70;
	h_iCost = 3;
	h_feature.pushBack(Feature_HEIYE);
	h_occupation.pushBack(Occupation_SHENDUNZHANSHI);
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 850;
			h_iAD = 80;
			h_iSell = 3;
			ap = 250;
			break;
		case 2:
			h_iMaxHP = 1530;
			h_iAD = 144;
			h_iSell = 8;
			ap = 350;
			break;
		case 3:
			h_iMaxHP = 2754;
			h_iAD = 259;
			h_iSell = 21;
			ap = 700;
			break;
	}
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf, dbfe, bf, bfe);
	/*���ı���ȡ����˵��*/
}

void Lee::UseSkill()
{
	if (h_isDead) {   //����˲���������˺�������
		return;
	}
	Target->hurtMe(h_Skill.damageAD, AD_damage);

	Target->hurtMe(h_Skill.damageAP, AP_damage);

	Target->hurtMe(h_Skill.damageREAL, REAL_damage);
	setfCurMANA(0);//�����ͷź���������
	/*����*/
	if (!(Target->h_isDebuff))
		return;
	int sp = Target->getfSpeed();
	sp = sp * 0.5;
	Target->setfSpeed(sp);
	
}

Naideli::Naideli(int level)
{
	int ad = 0, ap = 0, real = 0, regen = 0, dbf = 0, dbfe = 0, bf = 0, bfe = 0;
	h_ID = HERO_Naideli;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "�ε���";
	h_iLevel = level;
	h_iAR = 35;
	h_iMR = 35;
	h_iAS = 0.9;
	h_iCRIT = 0.25;
	h_iARG = 3;
	h_iInitMANA = 0;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 60;
	h_iCost = 3;
	h_feature.pushBack(Feature_LIMING);
	h_occupation.pushBack(Occupation_SHENDUNZHANSHI);
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 800;
			h_iAD = 65;
			h_iSell = 3;
			ap = 350;
			dbfe = 30;
			break;
		case 2:
			h_iMaxHP = 1440;
			h_iAD = 117;
			h_iSell = 8;
			ap = 500;
			dbfe = 40;
			break;
		case 3:
			h_iMaxHP = 2592;
			h_iAD = 211;
			h_iSell = 21;
			ap = 750;
			dbfe = 50;
			break;
	}
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf, dbfe, bf, bfe);
	/*���ı���ȡ����˵��*/
}

void Naideli::UseSkill()
{
	if (h_isDead) {   //����˲���������˺�������
		return;
	}
	Target->hurtMe(h_Skill.damageAD, AD_damage);

	Target->hurtMe(h_Skill.damageAP, AP_damage);

	Target->hurtMe(h_Skill.damageREAL, REAL_damage);
	setfCurMANA(0);//�����ͷź���������
	/*����*/
	if (!(Target->h_isDebuff))
		return;
	int sp = Target->getfSpeed();
	sp = sp * (100-h_Skill.DebuffEffect)/100;
	Target->setfSpeed(sp);
}

Pansen::Pansen(int level)
{
	int ad = 0, ap = 0, real = 0, regen = 0, dbf = 0, dbfe = 0, bf = 0, bfe = 0;
	h_ID = HERO_Pansen;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "��ɭ";
	h_iLevel = level;
	h_iAR = 45;
	h_iMR = 45;
	h_iAS = 0.75;
	h_iCRIT = 0.25;
	h_iARG = 1;
	h_iInitMANA = 30;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 60;
	h_iCost = 3;
	h_feature.pushBack(Feature_TULONG);
	h_occupation.pushBack(Occupation_SHENDUNZHANSHI);
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 750;
			h_iAD = 70;
			h_iSell = 3;
			ad = h_iAD*4;
			bfe = 65;
			break;
		case 2:
			h_iMaxHP = 1350;
			h_iAD = 126;
			h_iSell = 8;
			ad = h_iAD * 5;
			bfe = 65;
			break;
		case 3:
			h_iMaxHP = 2430;
			h_iAD = 227;
			h_iSell = 21;
			ad = h_iAD * 6.5;
			bfe = 65;
			break;
	}
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf, dbfe, bf, bfe);
	/*���ı���ȡ����˵��*/
}

void Pansen::UseSkill()
{
	if (h_isDead) {   //����˲���������˺�������
		return;
	}
	Target->hurtMe(h_Skill.damageAD, AD_damage);

	Target->hurtMe(h_Skill.damageAP, AP_damage);

	Target->hurtMe(h_Skill.damageREAL, REAL_damage);
	int ar = getfAR(), mr = getfMR();
	ar += h_Skill.BuffEffect, mr += h_Skill.BuffEffect;
	setfAR(ar);
	setfMR(mr);
	setfCurMANA(0);//�����ͷź���������
}

Ruiwen::Ruiwen(int level)
{
	int ad = 0, ap = 0, real = 0, regen = 0, dbf = 0, dbfe = 0, bf = 0, bfe = 0;
	h_ID = HERO_Ruiwen;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "����";
	h_iLevel = level;
	h_iAR = 35;
	h_iMR = 35;
	h_iAS = 0.8;
	h_iCRIT = 0.25;
	h_iARG = 1;
	h_iInitMANA = 0;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 40;
	h_iCost = 3;
	h_feature.pushBack(Feature_LIMING);
	h_occupation.pushBack(Occupation_ZHENGFUZHE);
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 850;
			h_iAD = 70;
			h_iSell = 3;
			ap = 100;
			bfe = 90;
			dbf = Debuff_stun;
			dbfe = 1;
			break;
		case 2:
			h_iMaxHP = 1530;
			h_iAD = 126;
			h_iSell = 8;
			ap = 200;
			bfe = 100;
			dbf = Debuff_stun;
			dbfe = 1;
			break;
		case 3:
			h_iMaxHP = 2754;
			h_iAD = 227;
			h_iSell = 21;
			ap = 500;
			bfe = 150;
			dbf = Debuff_stun;
			dbfe = 1;
			break;
	}
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf, dbfe, bf, bfe);
	/*���ı���ȡ����˵��*/
}

void Ruiwen::UseSkill()
{
	if (h_isDead) {   //����˲���������˺�������
		return;
	}
	Target->hurtMe(h_Skill.damageAD, AD_damage);

	Target->hurtMe(h_Skill.damageAP, AP_damage);

	Target->hurtMe(h_Skill.damageREAL, REAL_damage);

	int ad = getfAD();
	ad = ad * (h_Skill.BuffEffect + 100) / 100;
	setfAD(ad);

	setfCurMANA(0);//�����ͷź���������
	if (!(Target->h_isDebuff))
		return;
	/*ѣ��*/
	Target->h_Debuff = h_Skill.Debuff;
	Target->h_DebuffEffect = h_Skill.DebuffEffect;
}

Lux::Lux(int level)
{
	int ad = 0, ap = 0, real = 0, regen = 0, dbf = 0, dbfe = 0, bf = 0, bfe = 0;
	h_ID = HERO_Lux;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "����˿";
	h_iLevel = level;
	h_iAR = 20;
	h_iMR = 20;
	h_iAS = 0.6;
	h_iCRIT = 0.25;
	h_iARG = 4;
	h_iInitMANA = 30;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 70;
	h_iCost = 3;
	h_feature.pushBack(Feature_SHENGGUANGWEISHI);
	h_occupation.pushBack(Occupation_MISHUSHI);
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 600;
			h_iAD = 40;
			h_iSell = 3;
			ap = 100;
			bfe = 100;
			dbf = Debuff_root;
			dbfe = 2;
			break;
		case 2:
			h_iMaxHP = 1080;
			h_iAD = 72;
			h_iSell = 8;
			ap = 200;
			bfe = 250;
			dbf = Debuff_root;
			dbfe = 3;
			break;
		case 3:
			h_iMaxHP = 1944;
			h_iAD = 130;
			h_iSell = 21;
			ap = 500;
			bfe = 450;
			dbf = Debuff_root;
			dbfe = 5;
			break;
	}
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf, dbfe, bf, bfe);
	/*���ı���ȡ����˵��*/
}

void Lux::UseSkill()
{
	if (h_isDead) {   //����˲���������˺�������
		return;
	}
	h_Shield = h_Skill.BuffEffect;

	setfCurMANA(0);//�����ͷź���������
	if (!(Target->h_isDebuff))
		return;
	/*����*/
	Target->h_Debuff = h_Skill.Debuff;
	Target->h_DebuffEffect = h_Skill.DebuffEffect;
}

Lulu::Lulu(int level)
{
	int ad = 0, ap = 0, real = 0, regen = 0, dbf = 0, dbfe = 0, bf = 0, bfe = 0;
	h_ID = HERO_Lulu;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "¶¶";
	h_iLevel = level;
	h_iAR = 25;
	h_iMR = 25;
	h_iAS = 0.6;
	h_iCRIT = 0.25;
	h_iARG = 3;
	h_iInitMANA = 40;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 70;
	h_iCost = 3;
	h_feature.pushBack(Feature_XIAOEMO);
	h_occupation.pushBack(Occupation_MISHUSHI);
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 600;
			h_iAD = 40;
			h_iSell = 3;
			ap = 100;
			bfe = 200;
			dbf = Debuff_stun;
			dbfe = 1;
			break;
		case 2:
			h_iMaxHP = 1080;
			h_iAD = 72;
			h_iSell = 8;
			ap = 200;
			bfe = 300;
			dbf = Debuff_stun;
			dbfe = 1;
			break;
		case 3:
			h_iMaxHP = 1944;
			h_iAD = 130;
			h_iSell = 21;
			ap = 400;
			bfe = 500;
			dbf = Debuff_stun;
			dbfe = 1;
			break;
	}
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf, dbfe, bf, bfe);
	/*���ı���ȡ����˵��*/
}

void Lulu::UseSkill()
{
	if (h_isDead) {   //����˲���������˺�������
		return;
	}
	Target->hurtMe(h_Skill.damageAD, AD_damage);

	Target->hurtMe(h_Skill.damageAP, AP_damage);

	Target->hurtMe(h_Skill.damageREAL, REAL_damage);

	h_Shield = h_Skill.BuffEffect;
	setfCurMANA(0);//�����ͷź���������
	if (!(Target->h_isDebuff))
		return;
	/*����*/
	Target->h_Debuff = h_Skill.Debuff;
	Target->h_DebuffEffect = h_Skill.DebuffEffect;
}

Jiela::Jiela(int level)
{
	int ad = 0, ap = 0, real = 0, regen = 0, dbf = 0, dbfe = 0, bf = 0, bfe = 0;
	h_ID = HERO_Jiela;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "���";
	h_iLevel = level;
	h_iAR = 20;
	h_iMR = 20;
	h_iAS = 0.7;
	h_iCRIT = 0.25;
	h_iARG = 4;
	h_iInitMANA = 40;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 80;
	h_iCost = 3;
	h_feature.pushBack(Feature_LONGZU);
	h_occupation.pushBack(Occupation_FASHI);
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 600;
			h_iAD = 40;
			h_iSell = 3;
			ap = 250;
			dbfe = 25;
			break;
		case 2:
			h_iMaxHP = 1080;
			h_iAD = 72;
			h_iSell = 8;
			ap = 400;
			dbfe = 30;
			break;
		case 3:
			h_iMaxHP = 1944;
			h_iAD = 130;
			h_iSell = 21;
			ap = 700;
			dbfe = 35;
			break;
	}
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf, dbfe, bf, bfe);
	/*���ı���ȡ����˵��*/
}

void Jiela::UseSkill()
{
	if (h_isDead) {   //����˲���������˺�������
		return;
	}
	Target->hurtMe(h_Skill.damageAD, AD_damage);

	Target->hurtMe(h_Skill.damageAP, AP_damage);

	Target->hurtMe(h_Skill.damageREAL, REAL_damage);
	int mr = Target->getfMR();
	mr = mr * (100 - h_Skill.DebuffEffect) / 100;
	Target->setfMR(mr);
	setfCurMANA(0);//�����ͷź���������
}

Yasuo::Yasuo(int level)
{
	int ad = 0, ap = 0, real = 0, regen = 0, dbf = 0, dbfe = 0, bf = 0, bfe = 0;
	h_ID = HERO_Yasuo;
	//h_iModelID
	//h_iPlayerCode
	h_sName = "����";
	h_iLevel = level;
	h_iAR = 35;
	h_iMR = 35;
	h_iAS = 1;
	h_iCRIT = 0.25;
	h_iARG = 1;
	h_iInitMANA = 0;
	h_iCurMANA = h_iInitMANA;
	h_iMaxMANA = 40;
	h_iCost = 3;
	h_feature.pushBack(Feature_HEIYE);
	h_occupation.pushBack(Occupation_ZHENGFUZHE);
	switch (h_iLevel) {
		case 1:
			h_iMaxHP = 800;
			h_iAD = 60;
			h_iSell = 3;
			ap = 300;
			real = 30;
			bfe = 10;
			break;
		case 2:
			h_iMaxHP = 1440;
			h_iAD = 108;
			h_iSell = 8;
			ap = 400;
			real = 40;
			bfe = 15;
			break;
		case 3:
			h_iMaxHP = 2592;
			h_iAD = 194;
			h_iSell = 21;
			ap = 750;
			real = 75;
			bfe = 25;
			break;
	}
	h_iCurHP = h_iMaxHP;
	h_Skill.setSkill(ad, ap, real, regen, dbf, dbfe, bf, bfe);
	/*���ı���ȡ����˵��*/
}

void Yasuo::UseSkill()
{
	if (h_isDead) {   //����˲���������˺�������
		return;
	}
	Target->hurtMe(h_Skill.damageAD, AD_damage);

	Target->hurtMe(h_Skill.damageAP, AP_damage);

	Target->hurtMe(h_Skill.damageREAL, REAL_damage);
	int ad = Target->getfAD();
	ad += h_Skill.BuffEffect;
	Target->setfAD(ad);
	setfCurMANA(0);//�����ͷź���������
}