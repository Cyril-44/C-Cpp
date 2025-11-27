/* Subtask points:
    1: 10
    2: 15 (depends on 1)
    3: 10
    4: 30 (depends on 3)
    5: 35 (depends on 2 and 4)
*/

if @status1 == AC; then @s1 = 10; else @s1 = 0; fi
if @status1 == AC and @status2 == AC; then @s2 = 15; else @s2 = 0; fi
if @status3 == AC; then @s3 = 10; else @s3 = 0; fi
if @status3 == AC and @status4 == AC; then @s4 = 20; else @s4 = 0; fi
if @status4 == AC and @status5 == AC; then @s5 = 10; else @s5 = 0; fi
if @status2 == AC and @status5 == AC and @status6 == AC; then @s6 = 35; else @s6 = 0; fi

@total_score = @s1 + @s2 + @s3 + @s4 + @s5 + @s6;

if @total_score == 100; then
     @final_status = AC;
else
     @final_status = UNAC;
fi

@final_time = @time1 + @time2 + @time3 + @time4 + @time5 + @time6;

@final_memory = @memory1 + @memory2 + @memory3 + @memory4 + @memory5 + @memory6;
