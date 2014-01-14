class dynamic_win {
private:
    // Members
    int start_pnt, end_pnt; //Start and End points of the windows
    double probability; //Window's probability. Only the probability of the gesture winner
    int size_growth; //Size of growth of the windows
public:
    // Constructor and destructor
    dynamic_win(int start_pnt2 = 0, int end_pnt2 = 0, double probability2= 0.0, int size_growth2 = 0)
        : start_pnt(start_pnt2), end_pnt(end_pnt2), probability(probability2), size_growth(size_growth2) {};
    // Functions members
    int get_start_pnt();
    int get_end_pnt();
    double get_probability();
    void set_start_pnt(int start_pnt2);
    void set_end_pnt(int end_pnt2);
    void set_probability(double probability2);
};
