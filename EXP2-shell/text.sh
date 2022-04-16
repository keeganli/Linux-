#! /usr/bin/env bash
function main(){
    echo "============================================";
    echo "     Welcome to the Attendance System";
    echo "============================================";

    # init and create the file;
    if [[ ! -e UserInfo.dat ]]; then  #-e 判断对象是否存在 
            touch userinfo.dat   #保存用户名和密码
         chmod 777 UserInfo.dat #若要rwx属性则4+2+1=7；
    fi
    if [[ ! -e KaoqinInfo.dat ]]; then
        touch KaoqinInfo.dat   #保存迟到早退信息
       chmod 777 KaoqinInfo.dat
    fi
    if [[ ! -e KaoqinInfoCheckOut.dat ]]; then
        touch KaoqinInfoCheckOut.dat   #保存迟到早退信息
       chmod 777 KaoqinInfoCheckOut.dat
    fi
    Choose;
    read -p "Input your option : " choice
    if [[ $choice -eq 1 ]];
        then 
            read -p "Account Name : " username
            read -p "Password : " password
            Logining $username $password;
            menu $username;
    elif [[ $choice -eq 2 ]];
        then
            read -p "Register Account Name : " username
            read -p "Register Account Password : " password
            Register $username;
    fi

}
function Choose(){
    echo "============================================";
    echo " 1 : Sign                     2 : Register"
    echo "============================================";
}
function signIn(){
    # Choose;
    # read -p "Input your option : " cho
    if [[ $cho -eq 1 ]];
        then 
            # read -p "Account Name : " username
            # read -p "Password : " password
            Logining $username $password;
        else
            read -p "Register Account Name : " username
            read -p "Register Account Password : " password
            Register $username $password;
    fi
}
function Register(){
    echo $username:$password >> UserInfo.dat
    echo "Please Sign In or Input :wq To Quit"
    read -p "Acount Name or Quit: " infos
    if [[ $infos ==  ":wq" ]];
        then 
            return 0;
        else
            read -p "Account Password : " password;
            menu $username
    fi
}
function Logining(){
    while read line
        do
            if [[ "$line" == "$username:$password" ]]; 
            then 
                return 0
            fi
    done < UserInfo.dat
    echo "Wrong Accout Name or Password";
    read -p "Accout Name : " username
    read -p "Accout Password : " password
    Logining $username $password
}
function menu(){
      
    while [[ 1 == 1 ]];
    do
        echo "";
        echo "                       ***Menu***                           ";
        echo "============================================================";
        echo "         (1)            CheckIn                             ";
        echo "         (2)           CheckOuts                            ";
        echo "         (3)   View attendance information                  ";
        echo "         (4)             Quit                               ";
        echo "============================================================";
        read -p "Please Input Your Choice : " ans
        case $ans in
            1 )
                CheckIn $username; ;;               
            2 )
                CheckOut $username; ;;
            3 )
                Status $username; ;;
            4 )
               exit 0; ;;
            * )
                echo -e "Please Choose Options!\n"
       esac
    done
}
function CheckIn(){
    hour=`date +%H`
    if [[ $hour -gt 8 ]]; then
        echo "";
        echo "-------------------------------------";
        echo "You're late for work !  Recorded!"
        echo "$username : Lated Time : `date`" >> KaoqinInfo.dat
    else
        echo "You Have Successfully Signed! Have A Nice Day!"
    fi
}
function CheckOut(){
    echo "CheckOut Successful"
     hour=`date +%H`
     if [[ $hour -lt 18 ]]; then
         echo "You are leaving early now! Recorded!"
         echo "$username : CheckOut Early : Time:`date`" >> KaoqinInfoCheckOut.dat
     fi
     main;
}
function Status(){
    while [[ 1 == 1 ]]
    do 
        echo "";
        echo "        ***Query Service***          ";
        echo "======================================";
        echo "     (1)    Late Arrivals            ";
        echo "     (2)   Early Departures           ";
        echo "     (3)        Quit                 ";
        echo "======================================";
        read -p "PLease Input Your Options : " chos
        case $chos in
            1 ) 
                QuerySer $username; ;;
            2 ) 
                EarlyDep $username; ;;
            3 ) 
                return 0; ;;
            * )
                echo -e "Please Choose Options!\n"
        esac
    done
            
}
function EarlyDep () { 
    grep  "$username" KaoqinInfoCheckOut.dat
}

function QuerySer(){
    grep "$username" KaoqinInfo.dat;
}
main