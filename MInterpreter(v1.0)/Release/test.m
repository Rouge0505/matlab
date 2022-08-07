%本测试包含了当前版本支持的全部语法
%运行后，将在当前工作路径/Debug下生成过程文件，请注意清理
clear
clc
index = 1;		%计数变量
product = 1;	%各数乘积
sum = 0;		%各数求和
%是啥不重要，只用来测试运算和控制语句
while index < 5 && sum < 10
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
%是啥不重要，只用来测试计算语句
(index>=1||~0~= sum)+product+(-.2++3-5.4)*2.1^ 5
whatever=ans
