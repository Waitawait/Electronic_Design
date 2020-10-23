N=50; %������  
   
    Xkf=zeros(1,N);%�˲������״̬��Ҳ�й���ֵ
    Z=txtread('data.txt');%�ɼ���ѹֵ
    P=zeros(1,N);%Э�������


   
    %����
    R=0.09;

    V=sqrt(R)*randn(1,N); %���������ķ���ֲ�
   %ϵͳ����
   F=1;%״̬ת�ƾ���
   G=1;%������������
   H=1;%�۲����
   I=eye(1); %ϵͳΪһά
   %%ģ������������
   for k=2:N

       
       X_pre(k)=F*Xkf(k-1); %״̬Ԥ�⣬��ʱֵ
       P_pre(k)=F*P(k-1)*F'+ G*Q*G';%Э����Ԥ��                                           
       Kg(k)=P_pre(k)*H'*inv(H*P_pre(k)*H'+ R);%����kalman������  
       e=Z(k)-H*X_pre(k); %������Ϣ
       Xkf(k)=X_pre(k)+Kg(k)*e; %״̬���� 
       P(k)=(I-Kg(k)*H)*P_pre(k)*(I-Kg(k)*H)'+Kg(k)*R*Kg(k)'; %Э�������
  
   end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
       %%���������
       Err_messure=zeros(1,N);%���������¶ȼƲ���ֵ����ֵ�����
       Err_kalman=zeros(1,N);%���������˲����ֵ����ֵ���

       for k=1:N
            Err_messure(k)=abs(Z(k)-X(k));
            Err_kalman(k)=abs(Xkf(k)-X(k));
       end
       
       t=1:N;
       
       figure;
       plot(t,Z,'-ko',t,Xkf,'-g*');
       legend('����ֵ','KLM�˲���ֵ');
       xlabel('�ɼ�����');
       ylabel('�ɼ���ֵ');
       figure;
       plot(t,Err_messure,'-b.',t,Err_kalman,'-k*');
       legend('�ɼ����','��ֵ���');
       xlabel('�ɼ�����');
       ylabel('��ֵ');
       
