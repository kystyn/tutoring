S = 0
d = 10001
i = 0
>> N
while i < N:
	>> a, b
	S += max(a, b)
	if |b - a| < d and math.fabs(b - a) % 3 != 0:
		d = |b - a| 
	i += 1

if S % 3 != 0:
	<< S
else:
	if d == 10001:
		<< 0
	else:
		<< S - d
