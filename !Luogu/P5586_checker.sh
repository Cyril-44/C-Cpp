
for ((i=1; 1; i++)) do
    echo Test Case $i:;
    seed=$(od -An -N4 -tu4 /dev/urandom | tr -d ' ');
    python3 P5586_gen.py "$seed" 7 7;
    if ./P5586 < P5586.in > P5586.out; then
        if diff -b P5586.ans P5586.out >/dev/null; then
        echo -e "\e[32mAccepted!\e[0m";
        else
            echo -e "\e[1;31mWrong Answer!\e[0m";
            exit 1;
        fi
    else
        echo -e "\e[1;35mRuntime Error!\e[0m";
        exit 2;
    fi
done