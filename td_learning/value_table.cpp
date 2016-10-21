#include "value_table.h"

using namespace std;

value_table::value_table(){
    for(int i=0;i<4;++i){
        this->value_row[i] = new long double[1 << 20];
        this->value_column[i] = new long double[1 << 20];
    }
    for(int i=0;i<(1<<20);++i){
        for(int j=0;j<4;++j){
            this->value_row[j][i] = 0.0L;
            this->value_column[j][i] = 0.0L;
        }
    }
}

value_table::~value_table(){
    for(int i=0;i<4;++i){
        delete [] this->value_row[i];
        delete [] this->value_column[i];
    }
}

long double value_table::value(state_game s){
    if(s.valid == false)
        return 0.0L;

    long double rtn_value = 0.0L;
    //row
    for(int i=0;i<4;++i){
        rtn_value += this->value_row[i][s.bitboard[i]];
    }
    //column
    for(int i=0;i<4;++i){
        int this_column = state_game::column(i, s.bitboard);
        rtn_value += this->value_column[i][this_column];
    }
    return rtn_value;
}

void value_table::update(state_game &st, state_game &stn, long double learning_rate){
    long double reward = (long double)stn.score - (long double)st.score;

    long double vst = this->value(st);
    long double vstn = this->value(stn);

    long double diff_fix = learning_rate * (reward + vstn - vst) / 8.0L;

    for(int i=0;i<4;++i){
        this->value_row[i][st.bitboard[i]] += diff_fix;
        this->value_column[i][state_game::column(i, st.bitboard)] += diff_fix;
    }

    return;
}

void value_table::dump(const char* address){

    fstream out_tb(address, fstream::binary | fstream::out);

    if(out_tb.is_open() == false){
        cerr << "error dumping" <<endl;
        exit(-1);
    }

    //row
    for(int i=0;i<4;++i){
        for(int j=0;j<(1<<20);++j)
            out_tb.write((char*)&this->value_row[i][j], sizeof(long double));
    }
    //column
    for(int i=0;i<4;++i){
        for(int j=0;j<(1<<20);++j)
            out_tb.write((char*)&this->value_column[i][j], sizeof(long double));
    }

    out_tb.close();
    return;
}

void value_table::load(const char* address){
    fstream in_tb(address, fstream::binary | fstream::in);

    if(in_tb.is_open() == false){
        cerr << "error loading" <<endl;
        exit(-1);
    }

    //row
    for(int i=0;i<4;++i){
        for(int j=0;j<(1<<20);++j)
            in_tb.read((char*)&this->value_row[i][j], sizeof(long double));
    }
    //column
    for(int i=0;i<4;++i){
        for(int j=0;j<(1<<20);++j)
            in_tb.read((char*)&this->value_column[i][j], sizeof(long double));
    }

    in_tb.close();
    return;
}
