          .data
tab:	.asciiz	"\n"
space:	.asciiz	" "
str1:	.asciiz	"The max pooling : "
str2:	.asciiz	"The result of convolution : "
map:	.word	3,0,1,5,0,3,0, 
		2,6,2,4,3,0,3, 
		2,4,1,0,6,1,4, 
		3,0,1,5,0,3,0, 
		2,6,2,4,3,2,3, 
		2,4,1,0,6,2,1,
		2,6,2,4,4,0,3
	#输入
kernel:	.word	1, 0, -1,
     		1, 0, -1,
    		1, 0, -1
      	#卷积核
out:	.space 100	#输出结果
temp:	.space 1764	#定义一个卷积过程中的矩阵
          .text
        move	$t2,$0	#循环的i
        move	$t3,$0	#循环的j
loop1:	
	move	$t3,$0
Cov:	
	mul	$t0,$t2,45
	mul	$t1,$t3,9
	add	$s0,$t0,$t1
	#以上三步求卷积过程放置结果的坐标
	mul	$t4,$t2,7
	add	$s1,$t4,$t3
	#以上两步求卷积核进行的第一行运算输入的列数
	lw	$a2,map($s1)
	sw	$a2,temp($s0)
	move	$a2,$0
	move	$t1,$0
	move	$t0,$0
	#以上5行将结果存到卷积过程数组
	addi	$s1,$s1,4
	addi	$s0,$s0,4
	lw	$a2,map($s1)
	sw	$a2,temp($s0)
	move	$a2,$0
	#以上5行将结果存到卷积过程数组
	addi	$s1,$s1,4
	addi	$s0,$s0,4
	lw	$a2,map($s1)
	sw	$a2,temp($s0)
	move	$a2,$0
	#以上5行将结果存到卷积过程数组
	#以上七步求卷积核进行的第一行运算
	addi	$t6,$t2,4
	mul	$t7,$t6,7
	add	$s2,$t7,$t3
	#以上两步求卷积核进行的第二行运算输入的列数
	addi	$s0,$s0,4
	lw	$a2,map($s2)
	sw	$a2,temp($s0)
	move	$a2,$0
	#以上5行将结果存到卷积过程数组
	addi	$s2,$s2,4
	addi	$s0,$s0,4
	lw	$a2,map($s2)
	sw	$a2,temp($s0)
	move	$a2,$0
	#以上5行将结果存到卷积过程数组
	addi	$s2,$s2,4
	addi	$s0,$s0,4
	lw	$a2,map($s2)
	sw	$a2,temp($s0)
	move	$a2,$0
	#以上5行将结果存到卷积过程数组
	#以上七步求卷积核进行的第二行运算
	addi	$t6,$t2,8
	mul	$t7,$t6,7
	add	$s3,$t7,$t3
	#以上两步求卷积核进行的第三行运算输入的列数
	addi	$s0,$s0,4
	lw	$a2,map($s3)
	sw	$a2,temp($s0)
	move	$a2,$0
	#以上5行将结果存到卷积过程数组
	addi	$s3,$s3,4
	addi	$s0,$s0,4
	lw	$a2,map($s3)
	sw	$a2,temp($s0)
	move	$a2,$0
	#以上5行将结果存到卷积过程数组
	addi	$s3,$s3,4
	addi	$s0,$s0,4
	lw	$a2,map($s3)
	sw	$a2,temp($s0)
	move	$a2,$0
	#以上5行将结果存到卷积过程数组
	#以上七步求卷积核进行的第三行运算
	addi	$t3,$t3,4
	blt	$t3,20,Cov
	addi	$t2,$t2,4
	blt	$t2,20,loop1
	move	$t2,$0
	move	$t3,$0
	move	$t4,$0	#作为下一步第三重循环
	move	$t6,$0
	move	$t7,$0
loop2:
	move	$t3,$0
Cov2:
	move	$t4,$0
	move	$s5,$0	#temp
	#卷积后临时存储
	mul	$t6,$t2,45
	mul	$t7,$t3,9
	add	$s4,$t6,$t7	#wh
	#以上几步求卷积结果放置的坐标
next:
	add	$t0,$s4,$t4
	lw	$s0,temp($t0)
	#从卷积过程中取值
	lw	$s1,kernel($t4)
	#从卷积核中取值
	mul	$s2,$s1,$s0
	add	$s5,$s5,$s2
	addi	$t4,$t4,4
	blt	$t4,36,next
	sw	$s5,out($s6)
	addi	$t3,$t3,4
	addi	$s6,$s6,4
	blt	$t3,20,Cov2
	addi	$t2,$t2,4
	blt	$t2,20,loop2
	move	$t2,$0
	move	$t3,$0
la	$a0,str2
li	$v0,4
syscall
loop3:	
	move	$t3,$0
	la	$a0,tab
	li	$v0,4
	syscall
finish:	
	lw	$a0,out($t2)
	li	$v0,1
	syscall
	la	$a0,space
	li	$v0,4
	syscall
	addi	$t3,$t3,1
	addi	$t2,$t2,4
	blt	$t3,5,finish
	blt	$t2,100,loop3
	la	$a0,tab
	li	$v0,4
	syscall

#max pooling
	 move	$t4,$0  #指示数组第几个元素的i
	 move	$t0,$0	#当前最大数
loop4:   beq	$t4,100,final #判断结束循环指标
	 lw	$t3,out($t4)	#比较数
	 addi	$t4,$t4,4
	 blt	$t0,$t3,change
	 j	loop4
change:  move	$t0,$t3
	 j	loop4
final:   la	$a0,str1
	 li	$v0,4
	 syscall
	 addi	$a0,$t0,0
	 li	$v0,1
	 syscall
li	$v0,10
syscall
	
	
	
	
	

	
	
