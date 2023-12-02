#ifndef STATS_H
#define STATS_H
class Stats
{
public:
    void l1_hit();
    void l2_hit();
    void victim_hit();
    void l1_miss();
    void l2_miss();
    void victim_miss();
    double L1_miss_rate();
    double L2_miss_rate();
    double Victim_miss_rate();
    double AAT();
    
private:
	int missesL1 {}; 
	int missesL2 {}; 
	int missesVic {};
	int hitsL1 {};
	int hitsL2 {};
	int hitsVic {};
};
#endif