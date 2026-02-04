#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;
class Seat{
    public:
    int row_num;
    int col_num;
    bool is_booked;
    bool is_VIP;
    bool is_low=false;

    Seat(int row,int col){
        row_num=row;
        col_num=col;
        is_booked=false;
        is_VIP=true;
        if (row_num==0){
            is_low=true;
        }
        
    }


    void print(){
        cout<< "[ "<< (int) is_booked <<" ]";
    }
};
static int booking_number=0;

class Hall{
    public:
    string owner;
    int hall_no;
    int row; // 0->closest seat to screen 
    int col;
    vector<vector<Seat>> hall_seating={};
    double scale_up_factor_min=1; // slight increase of price scale factor
    double scale_up_factor_max=1; // Peak time price scale factor
    double scale_down_factor_min=1; // slight decrease of price scale factor
    double scale_down_factor_max=1; // Low demand time price scale factor
    double base_price_reg=200;
    double base_price_low=50;



    Hall(int hall_no,int row, int col){
        this->hall_no=hall_no;
        this->row=row;
        this->col=col;
        for (int i=0;i<row;i++){
            vector<Seat>* row= new vector<Seat>;
            for (int j=0;j<col;j++){
                Seat* seat1=new Seat(i,j);
                (*row).push_back(*seat1);         
            }
            hall_seating.push_back(*row);
        }
    }

    void view_hall(){
        for (int i=0;i<row;i++){
            for (int j=0;j<col;j++){
                hall_seating[i][j].print();
            }
            cout<<"\n";
        }
    }
    
    vector<int> seats_available(){
        int less_price_cat_seats=0;
        int high_price_cat_seats=0;
        int vip_seats=0;
        for (int i=0;i<row;i++){
            for (int j=0;j<col;j++){
                Seat current_seat= hall_seating[i][j];
                if (current_seat.is_booked==false){
                    if (i==0){
                        less_price_cat_seats++;
                    }
                    else{
                        high_price_cat_seats++;
                        if (current_seat.is_VIP){
                            vip_seats++;
                        }
                    }
                }
            }
        }
        vector<int> v= {less_price_cat_seats,high_price_cat_seats,vip_seats};
        return v;
    }
    double Price_calc(int num_seats,int mode){
        //pass
        return 0;
    }

    void manual_booking(User u,vector<Seat>book_seats, int mode){
        // mode 0 for low price, mode 1 for regular mode , mode 2 for VIP mode
        vector<int>v=this->seats_available();
        int n_req=book_seats.size(); // number of seats requested to be booked

        //checking if seats are available in the categories before proceeding to book the tickets
        bool valid=true;
        if (mode==0){
            if (n_req>v[0]){
                cout<<"All seats are booked in this category. Try other categories or other shows!";
                return;
            }
            else{
                
                for (Seat s: book_seats){
                    if (s.is_low==false){
                        cout<<"Wrong seat was chosen! Choose the correct seat from 0th row only!";
                        return;
                    }
                    if (s.is_booked==true){
                        cout<<"Seat is already booked! Try another seat!";
                        return;
                    }                  
                }
                
            }
        }
        else if (mode==1){
            if (n_req>v[1]){
                cout<<"All seats are booked in this category. Try other categories or other shows!";
                return;
            }
            else{
                for (Seat s: book_seats){
                    if (s.is_booked==true){
                        cout<<"Seat is already booked! Try another seat!";
                        return;
                    }                  
                }
            }
        }
        else{
            if (n_req>v[2]){
                cout<<"All seats are booked in this category. Try other categories or other shows!";
                return;
            }
            else{
                for (Seat s: book_seats){
                    if (s.is_VIP==false){
                        cout<<"Choosen seat is not VIP seat! Try another seat!";
                        return;
                    }
                    if (s.is_booked==true){
                        cout<<"Seat is already booked! Try another seat!";
                        return;
                    }                  
                }
            }
        }
        double price= Price_calc(book_seats.size(),mode);
        cout<<"The total price is:"<<price<<endl;
        bool paid=false;
        double amt;
        char ch='Y';
        while (ch=='Y' or ch=='y'){
            cout<<"Enter the amount to pay:";
            cin>>amt;
            if (amt==price){
                paid=true;
                break;
            }
            else{
                cout<<"Incorrect amount. Do you want to try again? (Y/N)";
                cin>>ch;                
            }

        }
        if (mode==0 or mode==1){
            if (paid){
                booking_number++;
                for (Seat s:book_seats){
                    s.is_booked=true;
                }
                cout<<"Booking done successfully!"<<endl;
                cout<<"Your Booking number is: "<<booking_number<<endl;
            }
        }
        else{
            if (paid){
                booking_number++;
                for(Seat s: book_seats){
                    int r= s.row_num;
                    int c=s.col_num;
                    hall_seating[r][c].is_booked=true;
                    if (r-1>=0){
                        hall_seating[r-1][c].is_booked=true;
                        if (c-1>=0){
                            hall_seating[r-1][c-1].is_booked=true;
                        }
                        if (c+1<col){
                            hall_seating[r-1][c+1].is_booked=true;
                        }
                    }
                    if (r+1<row){
                        hall_seating[r+1][c].is_booked=true;
                        if (c-1>=0){
                            hall_seating[r+1][c-1].is_booked=true;
                        }
                        if (c+1<col){
                            hall_seating[r+1][c+1].is_booked=true;
                        }
                    }
                    if (c-1>=0){
                        hall_seating[r][c-1].is_booked=true;
                    }
                    if (c+1<col){
                        hall_seating[r][c+1].is_booked=true;
                    }
                    
                    
                }
                cout<<"Booking done successfully!"<<endl;
                cout<<"Your Booking number is: "<<booking_number<<endl;
            }

        }
        
        
        
    }

    void auto_booking(User u, int num_seats, int mode){
        vector<int>v=this->seats_available();
        if (num_seats>v[mode]){
            cout<<"Invalid Request!";
            return;
        }
        for (vector<Seat> row: hall_seating){
            
        }

    }
    void set_min_scale_up(double factor){
        scale_up_factor_min=factor;
    }

    void set_max_scale_up(double factor){
        scale_up_factor_max=factor;
    }
    void set_min_scale_down(double factor){
        scale_down_factor_min=factor;
    }

    void set_max_scale_down(double factor){
        scale_down_factor_max=factor;
    }
    void set_base_price(double base_price_low, double base_price_reg){
        this->base_price_low=base_price_low;
        this->base_price_reg=base_price_reg;

    }    

};



class User{
    string password;
    public:
    string username;
    vector<int> Booking_history={};


};

int main(){
    Hall* h1= new Hall(1,5,5);
    h1->view_hall();
    vector<int> v= h1->seats_available();
    for (int i=0; i<v.size();i++){
        cout<<v[i]<<" ";
    }
    cout<< "\n";

    return 0;
}