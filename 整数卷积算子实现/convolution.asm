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
	#����
kernel:	.word	1, 0, -1,
     		1, 0, -1,
    		1, 0, -1
      	#�����
out:	.space 100	#������
temp:	.space 1764	#����һ����������еľ���
          .text
        move	$t2,$0	#ѭ����i
        move	$t3,$0	#ѭ����j
loop1:	
	move	$t3,$0
Cov:	
	mul	$t0,$t2,45
	mul	$t1,$t3,9
	add	$s0,$t0,$t1
	#���������������̷��ý��������
	mul	$t4,$t2,7
	add	$s1,$t4,$t3
	#�������������˽��еĵ�һ���������������
	lw	$a2,map($s1)
	sw	$a2,temp($s0)
	move	$a2,$0
	move	$t1,$0
	move	$t0,$0
	#����5�н�����浽�����������
	addi	$s1,$s1,4
	addi	$s0,$s0,4
	lw	$a2,map($s1)
	sw	$a2,temp($s0)
	move	$a2,$0
	#����5�н�����浽�����������
	addi	$s1,$s1,4
	addi	$s0,$s0,4
	lw	$a2,map($s1)
	sw	$a2,temp($s0)
	move	$a2,$0
	#����5�н�����浽�����������
	#�����߲������˽��еĵ�һ������
	addi	$t6,$t2,4
	mul	$t7,$t6,7
	add	$s2,$t7,$t3
	#�������������˽��еĵڶ����������������
	addi	$s0,$s0,4
	lw	$a2,map($s2)
	sw	$a2,temp($s0)
	move	$a2,$0
	#����5�н�����浽�����������
	addi	$s2,$s2,4
	addi	$s0,$s0,4
	lw	$a2,map($s2)
	sw	$a2,temp($s0)
	move	$a2,$0
	#����5�н�����浽�����������
	addi	$s2,$s2,4
	addi	$s0,$s0,4
	lw	$a2,map($s2)
	sw	$a2,temp($s0)
	move	$a2,$0
	#����5�н�����浽�����������
	#�����߲������˽��еĵڶ�������
	addi	$t6,$t2,8
	mul	$t7,$t6,7
	add	$s3,$t7,$t3
	#�������������˽��еĵ������������������
	addi	$s0,$s0,4
	lw	$a2,map($s3)
	sw	$a2,temp($s0)
	move	$a2,$0
	#����5�н�����浽�����������
	addi	$s3,$s3,4
	addi	$s0,$s0,4
	lw	$a2,map($s3)
	sw	$a2,temp($s0)
	move	$a2,$0
	#����5�н�����浽�����������
	addi	$s3,$s3,4
	addi	$s0,$s0,4
	lw	$a2,map($s3)
	sw	$a2,temp($s0)
	move	$a2,$0
	#����5�н�����浽�����������
	#�����߲������˽��еĵ���������
	addi	$t3,$t3,4
	blt	$t3,20,Cov
	addi	$t2,$t2,4
	blt	$t2,20,loop1
	move	$t2,$0
	move	$t3,$0
	move	$t4,$0	#��Ϊ��һ��������ѭ��
	move	$t6,$0
	move	$t7,$0
loop2:
	move	$t3,$0
Cov2:
	move	$t4,$0
	move	$s5,$0	#temp
	#�������ʱ�洢
	mul	$t6,$t2,45
	mul	$t7,$t3,9
	add	$s4,$t6,$t7	#wh
	#���ϼ�������������õ�����
next:
	add	$t0,$s4,$t4
	lw	$s0,temp($t0)
	#�Ӿ��������ȡֵ
	lw	$s1,kernel($t4)
	#�Ӿ������ȡֵ
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
	 move	$t4,$0  #ָʾ����ڼ���Ԫ�ص�i
	 move	$t0,$0	#��ǰ�����
loop4:   beq	$t4,100,final #�жϽ���ѭ��ָ��
	 lw	$t3,out($t4)	#�Ƚ���
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
	
	
	
	
	

	
	
