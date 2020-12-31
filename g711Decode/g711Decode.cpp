// g711Decode.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"


#include <windows.h>//���ͷ�ļ�Ҫ������ǰ�棬�������ʾ��Щ�����Ҳ�����һ����ʾ��102�����󣬸��˾���
#include <stdio.h>
#include <stdlib.h>
#include "g711.h"


#include <mmsystem.h>
#include <dsound.h>



//�������ÿ�
#pragma   comment(lib,"winmm.lib")
#pragma   comment(lib,"dsound.lib")  
#pragma   comment(lib,"dxguid.lib")



HRESULT hr;//���صĽ�����
DWORD g_dwNextWriteOffset ;

LPDIRECTSOUNDBUFFER8 g_lpDSBuffer8=NULL; //DS�Ĺٷ��������
LPDIRECTSOUNDNOTIFY8 g_lpDSNotify=0; //DS��ͨ��λ

LPDIRECTSOUND8 g_pDsd=0; //DS�ı���
DSBUFFERDESC dsBufDesc;//DS�Ļ�������--��������DS������������
LPDIRECTSOUNDBUFFER lpBuffer;//DS�Ļ���

#define buffer_size 800 //���岥�ŵ�ÿһƬ���嶼��800���ֽ�
#define MAX_AUDIO_BUF 4 //���Ż����֪ͨ����
#define BUFFERNOTIFYSIZE buffer_size/*8192*//*192000*/ //����֪ͨλ�õĴ�С����μ�DirectSoundӦ�ó��򿪷���������
#define SAMPLE_RATE      8000/*44100*/ //pcm �Ĳ�����  8k
#define N_CHANNEL          1/*2*/   //PCM ������ ��ͨ��
#define BITS_PER_SAMPLE    16  //ÿ��������ı�����

DSBPOSITIONNOTIFY g_arrayPosNotify[MAX_AUDIO_BUF];//����֪ͨλ�ñ�־������
HANDLE g_event[MAX_AUDIO_BUF];//DS�¼��ľ��

HANDLE g_decodeEvent;//�Լ������һ���¼�����Ҫ������⵱DS������һ����������ڴ�Ľ�ѹ������һ�����źŵı�־λ��Ȼ���ѹ����Ϳ��԰�
                    //��ѹ�õ���������buffer2�п���
HANDLE g_playEvent;//�Լ������һ���¼�����Ҫ������֪buffer2���ǲ��������ݣ�����������ݣ���ô����DS�Ĳ��Ż�����ȥ
HANDLE g_hPlay=NULL;

char g_copyBuf[buffer_size];
char g_decodeBuf[buffer_size];

BOOL g_bPlaying = FALSE; //�Ƿ����ڲ��ű�־λ

int ProcessBuffer(LPDIRECTSOUNDBUFFER8 g_pDSBuffer8, char *buffer);//������д��������


//�����߳�
DWORD WINAPI PlayPCMThread(LPVOID lpParame)
{
	g_bPlaying = TRUE;
	DWORD res=5;
	LPVOID lplockbuf;
	DWORD len;
	int flag=1;
	//��Ϊûʲô���ݺ��ͣ����������ڰ�BUFFERNOTIFYSIZE*3��С�Ļ���memsetΪ0
	g_lpDSBuffer8->Lock(0,BUFFERNOTIFYSIZE*3,&lplockbuf,&len,NULL,NULL,0);
	memset(lplockbuf,0x0,len);
	g_lpDSBuffer8->Unlock(lplockbuf,len,NULL,0);
	g_lpDSBuffer8->SetCurrentPosition(0);
	//ѭ���ţ���g_lpDSBuffer8�Լ��Ļ��棬һ��Ҫѭ����
	g_lpDSBuffer8->Play(0,0,DSBPLAY_LOOPING);
//	g_lpDSBuffer8->Play(0,0,0);

	g_dwNextWriteOffset=BUFFERNOTIFYSIZE*3;//��ȻҪ��дg_dwNextWriteOffset��ƫ��λ����
	while(g_bPlaying)//��������Էţ����е÷ŵĻ����ͼ�����DS�Ļ���
	{
		res = WaitForMultipleObjects (MAX_AUDIO_BUF, g_event, FALSE, INFINITE);
		if((res >=WAIT_OBJECT_0)&&(res <=WAIT_OBJECT_0+3))//������һ����Ҫ����� 
		{
		    //printf("res=%d\n",res);
			SetEvent(g_decodeEvent);//��g_hEvent_nsl�¼���Ϊ���ź�--˵��DS����һ���������ˣ��Ǹ�unpack_input�Ĺ����ֿ��Լ�����
			flag=ProcessBuffer(g_lpDSBuffer8,g_copyBuf);//�˺���ʵ���˴�copyBuf�п���BUFFERNOTIFYSIZE���ֽڵ�g_pDSBuffer8��ȥ
			res=5;
			if(flag==0)  g_bPlaying=false;
		}
	}
	return 1;
}

/*
 * Unpack input codes and pass them back as bytes.
 * Returns 1 if there is residual input, returns -1 if eof, else returns 0.
 */
int
unpack_input(
	unsigned char		*code,
	int			bits)
{
	static unsigned int	in_buffer = 0;
	static int		in_bits = 0;
	unsigned char		in_byte;

	if (in_bits < bits) {
		if (fread(&in_byte, sizeof (char), 1, stdin) != 1) {
			*code = 0;
			return (-1);
		}
		in_buffer |= (in_byte << in_bits);
		in_bits += 8;
	}
	*code = in_buffer & ((1 << bits) - 1);
	in_buffer >>= bits;
	in_bits -= bits;
	return (in_bits > 0);
}




int main(int argc, char* argv[])
{
	if( argc < 2 )
	{
		printf("Usage:\n");
		printf("g711Decode.exe filename a --G711A\n") ;
		printf("g711Decode.exe filename u --G711U\n") ;
		return 0 ;
	}
	SetConsoleTitle("G.711 audio file DecodeAndPlay");//���ô�������
	if (FAILED(hr =DirectSoundCreate8(NULL,&g_pDsd,NULL)))
		return false;
	if (FAILED(hr=g_pDsd->SetCooperativeLevel(FindWindow(NULL,"G.711 audio file DecodeAndPlay"),DSSCL_NORMAL)))
	{
		return false;
	}
	printf("G.711 audio file is playing now......\n");
	memset(&dsBufDesc,0,sizeof(dsBufDesc));
	dsBufDesc.dwSize=sizeof(dsBufDesc);
	dsBufDesc.dwFlags=DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLPOSITIONNOTIFY |DSBCAPS_GETCURRENTPOSITION2;
	dsBufDesc.dwBufferBytes=MAX_AUDIO_BUF*BUFFERNOTIFYSIZE; 
	dsBufDesc.lpwfxFormat=(WAVEFORMATEX*)malloc(sizeof(WAVEFORMATEX));
	dsBufDesc.lpwfxFormat->wFormatTag=WAVE_FORMAT_PCM; /* format type */  
	(dsBufDesc.lpwfxFormat)->nChannels=N_CHANNEL; /* number of channels (i.e. mono, stereo...) */         
	(dsBufDesc.lpwfxFormat)->nSamplesPerSec=SAMPLE_RATE; /* sample rate */    
	(dsBufDesc.lpwfxFormat)->nAvgBytesPerSec=SAMPLE_RATE*(BITS_PER_SAMPLE/8)*N_CHANNEL; /* for buffer estimation */    
	(dsBufDesc.lpwfxFormat)->nBlockAlign=(BITS_PER_SAMPLE/8)*N_CHANNEL; /* block size of data */       
	(dsBufDesc.lpwfxFormat)->wBitsPerSample=BITS_PER_SAMPLE; /* number of bits per sample of mono data */    
	(dsBufDesc.lpwfxFormat)->cbSize=0;	


	//����DirectSound����������
	if(DS_OK != (hr=(g_pDsd->CreateSoundBuffer(&dsBufDesc,&lpBuffer,NULL))))
	{   
		switch(hr)
		{
			case DSERR_ALLOCATED:printf("DSERR_ALLOCATED\n");break;
			case DSERR_BADFORMAT:printf("DSERR_BADFORMAT\n");break;
				/*		case DSERR_BUFFERTOOSMALL:printf("DSERR_BUFFERTOOSMALL\n");break;*/
			case DSERR_CONTROLUNAVAIL:printf("DSERR_CONTROLUNAVAIL\n");break;
				/*		case DSERR_DS8_REQUIRED:printf("DSERR_DS8_REQUIRED\n");break;*/
			case DSERR_INVALIDCALL:printf("DSERR_INVALIDCALL\n");break;
			case DSERR_INVALIDPARAM:printf("DSERR_INVALIDPARAM\n");break;
			case DSERR_NOAGGREGATION:printf("DSERR_NOAGGREGATION\n");break;
			case DSERR_OUTOFMEMORY:printf("DSERR_OUTOFMEMORY\n");break;
			case DSERR_UNINITIALIZED:printf("DSERR_UNINITIALIZED\n");break;
			case DSERR_UNSUPPORTED:printf("DSERR_UNSUPPORTED\n");break;
		}
		return FALSE;
	}

	if (FAILED(hr =(lpBuffer->QueryInterface(IID_IDirectSoundBuffer8,(LPVOID*)&g_lpDSBuffer8))))
	{
		return false;
	}
	lpBuffer->Release();

	//����DirectSound֪ͨ����
	if(FAILED(hr=(g_lpDSBuffer8->QueryInterface(IID_IDirectSoundNotify,(LPVOID*)&g_lpDSNotify))))
	{
		switch(hr)
		{
			case E_NOINTERFACE:printf("E_NOINTERFACE hr\n");break;
			default:printf("an unknow error hr,%d\n",hr);break;
		}
		return false ;
	}

	for(int i =0;i<MAX_AUDIO_BUF;i++)
	{
		g_arrayPosNotify[i].dwOffset =i*BUFFERNOTIFYSIZE;
		g_event[i]=::CreateEvent(NULL,false,false,NULL);//Ĭ�ϰ�ȫ�����Զ����ƣ���ʼΪ���źţ�Ĭ��ϵͳ������
		g_arrayPosNotify[i].hEventNotify=g_event[i];
	}	
	g_lpDSNotify->SetNotificationPositions(MAX_AUDIO_BUF,g_arrayPosNotify);
	g_lpDSNotify->Release();

	//��ʼ��buf
	memset(g_decodeBuf,0,buffer_size);
	memset(g_copyBuf,0,buffer_size);


	FILE * inpf;
	FILE * outf;

	inpf = fopen(argc == 2 ? argv[0] : argv[1], "rb");
	if( inpf == NULL )
	{
		printf("G.711 file cant open\n");
		return 1 ;
	}
	outf = fopen("nemozhu.pcm", "wb");
	int nFileType = stricmp( argc == 2 ? argv[1] : argv[2] , "a" ) == 0 ? 8 : 0 ;
	int	out_size =2;


	unsigned char* Buf;
	Buf = (unsigned char*)calloc(1024, sizeof(char));
	int nalLen = 0;
	int sumLen=0;

    //�Լ����һ���¼��ı���
	g_decodeEvent = ::CreateEvent(NULL,TRUE,TRUE,NULL);//Ĭ�ϰ�ȫ�����˹����ƣ���ʼΪ���źţ�Ĭ��ϵͳ������

	g_playEvent = ::CreateEvent(NULL,TRUE,FALSE,NULL);//Ĭ�ϰ�ȫ�����˹����ƣ���ʼΪ���źţ�Ĭ��ϵͳ������

	//����һ��play���߳�
	g_hPlay=CreateThread(0,0,PlayPCMThread,NULL,NULL,NULL);//�������ʧ�ܣ���ô����ֵ��NULL

	while(!feof(inpf))
	{
		nalLen =fread(Buf,1,1,inpf);
		int iLinear =0 ;
		if( nFileType == 0 )
			iLinear = ulaw2linear(Buf[0]); //g711u
		else
			iLinear = alaw2linear(Buf[0]); //g711a

		memcpy(g_decodeBuf+sumLen,&iLinear,out_size);//������buf
		fwrite(&iLinear,1,out_size,outf);//д���ļ�

		sumLen +=out_size;
		if (sumLen>=buffer_size-1)
		{
			sumLen =0;
		    WaitForSingleObject(g_decodeEvent,INFINITE);//���޵ȴ�g_hEvent_nsl���ź�
			memcpy(g_copyBuf,g_decodeBuf,buffer_size);

			SetEvent(g_playEvent);
			//if(buffer2_ready==true) {memcpy(buffer2,buffer1,buffer_size);buffer2_ready=false;}
			ResetEvent(g_decodeEvent);//��g_hEvent_nsl ����Ϊ���ź�״̬   
		}

	}
	
	if(inpf)
		fclose(inpf);

	if(outf)
		fclose(outf);

	g_bPlaying=false;

	printf("The audio file is end ! \n");

	return 0;
}


//һ��������ĺ�������Ҫ����һ���и����岥������֮�󣬾���DS�ĸ�д����ĵط�д��һ������
int ProcessBuffer(LPDIRECTSOUNDBUFFER8 g_lpDSBuffer8, char *buffer)
{
	VOID* pDSLockedBuffer = NULL;
	VOID* pDSLockedBuffer2 = NULL;
	DWORD dwDSLockedBufferSize;
	DWORD dwDSLockedBufferSize2;
	HRESULT hr; 
	hr=g_lpDSBuffer8->Lock(g_dwNextWriteOffset,BUFFERNOTIFYSIZE,&pDSLockedBuffer,&dwDSLockedBufferSize, &pDSLockedBuffer2,&dwDSLockedBufferSize2,0);
	if(hr == DSERR_BUFFERLOST)
	{
		//DS��������һ��
		g_lpDSBuffer8->Restore();
		//����lockһ��
        g_lpDSBuffer8->Lock(g_dwNextWriteOffset,BUFFERNOTIFYSIZE,&pDSLockedBuffer,&dwDSLockedBufferSize,&pDSLockedBuffer2,&dwDSLockedBufferSize2,0);
	}
	if(SUCCEEDED(hr))
	{
	       //if(buffer2_new==false) printf("buffer2_new====false\n");
		   //else printf("buffer2_new=============true\n");
		WaitForSingleObject(g_playEvent,500);//�ȵ�buffer2�б�Ϊ�����ݣ���������500ms
		memcpy((BYTE*)pDSLockedBuffer,buffer,buffer_size);//Ȼ���buffer�е����ݿ���DS��lockbuffer��ע������buffer=copybuffer
		ResetEvent(g_playEvent);//���°�g_playEvent����Ϊ���źţ�Ҳ����˵copybuffer�����ݲ���������
		g_dwNextWriteOffset+=dwDSLockedBufferSize;
		g_dwNextWriteOffset %= (BUFFERNOTIFYSIZE * MAX_AUDIO_BUF);//ʵ��һ������ѭ������ĵط�
		hr = g_lpDSBuffer8->Unlock(pDSLockedBuffer,dwDSLockedBufferSize,pDSLockedBuffer2,dwDSLockedBufferSize2);//unlock��һƬ���壬׼��������
	}
	return 1;
}
