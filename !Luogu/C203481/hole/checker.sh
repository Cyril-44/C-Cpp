for n in {1..7}
do
    cp hole$n.in hole.in
    result=$(/usr/bin/time -f "#$n: %es" ./hole)
    diff -b hole$n.ans hole.out > /dev/null
    if [ $? -ne 0 ]; then
    echo WA on \#$n
    read -p "Press any key to continue..."
    else
    echo Accepted
    fi
done