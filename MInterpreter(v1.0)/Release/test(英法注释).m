%This is only a test这是一个测试
index = 1;		%the index of integers
product = 1;	%the product of integers
sum = 0;		%the sum of integers
%n'import quoi, as long as it includes all syntaxes
while index < 5 & sum < 10
	sum = sum + index;
	product = product * index;
	if product > 20
		product = 20;
		break;
	else
		index = index + 1;
		continue;
	end
end
index
sum
product
%n'import quoi, as long as it includes all calculations
(index>=1|~0~= sum)+product+(-.2++3-5.4)*2.1^ 5
whatever=ans
