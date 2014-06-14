package com.boyaa.chat.demo;
	import android.R.bool;
import android.annotation.SuppressLint;
import android.app.Activity;  
import android.content.Context;

import java.net.HttpURLConnection;
import java.net.URL;  
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;  

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.media.AudioManager;
import android.media.SoundPool;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.webkit.WebSettings;
import android.webkit.WebSettings.LayoutAlgorithm;
import android.webkit.WebSettings.PluginState;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.ImageView;
import android.widget.TextView;
import android.view.KeyEvent;
import android.view.View.OnClickListener;
import android.view.WindowManager;
import android.widget.EditText;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;

import com.boyaa.chat.R;
import com.boyaa.push.lib.service.Client;
import com.boyaa.push.lib.service.ISocketResponse;
import com.boyaa.push.lib.service.Packet;

@SuppressLint({ "SimpleDateFormat", "SetJavaScriptEnabled", "UseSparseArrays" })
public class MainActivity extends Activity {

	private Client user=null;
//	private EditText ip;
	private TextView status;
	private WebView myWebView;
	
	private String camera_ip="192.168.43.1";
	private String board_ip="192.168.1.136";
	SoundPool player,play;
	HashMap<Integer,Integer> soundMap;
	
	private SensorManager sm;
	//需要两个Sensor
	private Sensor aSensor;
	private Sensor mSensor;
	float[] accelerometerValues = new float[3];
	float[] magneticFieldValues = new float[3];
	private static final String TAG = "sensor";

	int flag;
	int mode;
	int viewcount;
	int connected;
	 int degree;
	 Packet packet=new Packet();
	 
	ImageView imageView;
   
    
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		
		
	
		  
		  
		setContentView(R.layout.activity_main);
		
//		ip=(EditText) findViewById(R.id.ip);
		status=(TextView) findViewById(R.id.status);
		
//		ip.setText(board_ip);
		status.setText("off");
		
		flag=0;
		mode=0;
		viewcount=1;
		connected=0;
		
		
		initView();
		
		
		
		 soundMap=new HashMap<Integer,Integer>();
	        player=new SoundPool(1,AudioManager.STREAM_MUSIC,10);
	        play=new SoundPool(1,AudioManager.STREAM_MUSIC,10);
	        play.load(this, R.raw.test, 10);
	        soundMap.put(1,player.load(this, R.raw.test, 10));
	
        //显示
		
		user=new Client(this.getApplicationContext(),socketListener);
		
		
		sm = (SensorManager)getSystemService(Context.SENSOR_SERVICE);
		aSensor = sm.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
		mSensor = sm.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD);
		
		sm.registerListener(myListener, aSensor, SensorManager.SENSOR_DELAY_NORMAL);
		sm.registerListener(myListener, mSensor,SensorManager.SENSOR_DELAY_NORMAL);
		calculateOrientation();
	}
	
	
	final SensorEventListener myListener = new SensorEventListener() {
	public void onSensorChanged(SensorEvent sensorEvent) {
		
	if (sensorEvent.sensor.getType() == Sensor.TYPE_MAGNETIC_FIELD)
	magneticFieldValues = sensorEvent.values;
	if (sensorEvent.sensor.getType() == Sensor.TYPE_ACCELEROMETER)
		accelerometerValues = sensorEvent.values;
	calculateOrientation();
	}
	public void onAccuracyChanged(Sensor sensor, int accuracy) {}
	};

	
	
	public void onPause(){
		sm.unregisterListener(myListener);
		super.onPause();
	}	
	
	private  void calculateOrientation() {
	      float[] values = new float[3];
	      float[] R = new float[9];
	      SensorManager.getRotationMatrix(R, null, accelerometerValues, magneticFieldValues);	      
	      SensorManager.getOrientation(R, values);

	      // 要经过一次数据格式的转换，转换为度
	     
	      values[0] = (float) Math.toDegrees(values[0]);
	      degree=(int)values[0];
	      if(degree <0)
	    	  degree = 360+degree;
	      
	      Log.i(TAG, values[0]+"");
	      
//	      status.setText(degree+"");
	      //values[1] = (float) Math.toDegrees(values[1]);
	      //values[2] = (float) Math.toDegrees(values[2]);
	      
	      if(values[0] >= -5 && values[0] < 5){
	    	 Log.i(TAG, "正北");
	      }
	      else if(values[0] >= 5 && values[0] < 85){
	    	  Log.i(TAG, "东北");
	      }
	      else if(values[0] >= 85 && values[0] <=95){
	    	  Log.i(TAG, "正东");
	      }
	      else if(values[0] >= 95 && values[0] <175){
	    	  Log.i(TAG, "东南");
	      }
	      else if((values[0] >= 175 && values[0] <= 180) || (values[0]) >= -180 && values[0] < -175){
	    	  Log.i(TAG, "正南");
	      }
	      else if(values[0] >= -175 && values[0] <-95){
	    	  Log.i(TAG, "西南");
	      }
	      else if(values[0] >= -95 && values[0] < -85){
	    	  Log.i(TAG, "正西");
	      }
	      else if(values[0] >= -85 && values[0] <-5){
	    	  Log.i(TAG, "西北");
	      }
	    }
	
	     

	
	private void initView()
	{
		viewcount=1;
//		ip=(EditText) findViewById(R.id.ip);
		status=(TextView) findViewById(R.id.status);
		
//		ip.setText(board_ip);
		
		 imageView = (ImageView)findViewById(R.id.image_view);
		
		findViewById(R.id.open).setOnClickListener(listener);
		findViewById(R.id.comfrim).setOnClickListener(listener);
		findViewById(R.id.reset).setOnClickListener(listener);
		
		findViewById(R.id.start).setOnClickListener(listener);
		findViewById(R.id.comfrim).setVisibility(View.VISIBLE);
		
		findViewById(R.id.next).setOnClickListener(listener);
		findViewById(R.id.pattern1).setOnClickListener(listener);
		findViewById(R.id.pattern2).setOnClickListener(listener);
		findViewById(R.id.pattern3).setOnClickListener(listener);
		findViewById(R.id.pattern4).setOnClickListener(listener);
		
		findViewById(R.id.single).setOnClickListener(listener);
		findViewById(R.id.ddouble).setOnClickListener(listener);
		findViewById(R.id.multiple).setOnClickListener(listener);

		findViewById(R.id.pdiy).setOnClickListener(listener);

		findViewById(R.id.mback).setOnClickListener(listener);

		findViewById(R.id.pattern5).setOnClickListener(listener);
		findViewById(R.id.pattern6).setOnClickListener(listener);
		findViewById(R.id.pattern7).setOnClickListener(listener);
		findViewById(R.id.pattern8).setOnClickListener(listener);
		findViewById(R.id.pattern9).setOnClickListener(listener);
		findViewById(R.id.pattern10).setOnClickListener(listener);
		
//		findViewById(R.id.single).setOnClickListener(listener);

				
		if(flag==0)
		{
			findViewById(R.id.comfrim).setVisibility(View.INVISIBLE);
			findViewById(R.id.start).setVisibility(View.INVISIBLE);
			
			findViewById(R.id.pattern1).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern2).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern3).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern4).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern5).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern6).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern7).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern8).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern9).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern10).setVisibility(View.INVISIBLE);
			findViewById(R.id.pdiy).setVisibility(View.INVISIBLE);
			findViewById(R.id.mback).setVisibility(View.INVISIBLE);

			findViewById(R.id.single).setVisibility(View.INVISIBLE);
			findViewById(R.id.ddouble).setVisibility(View.INVISIBLE);
			findViewById(R.id.multiple).setVisibility(View.INVISIBLE);


			//			findViewById(R.id.single).setVisibility(View.VISIBLE);

		}
		else if(flag==1) //choose pattern
		{
			findViewById(R.id.single).setVisibility(View.VISIBLE);
			findViewById(R.id.ddouble).setVisibility(View.VISIBLE);
			findViewById(R.id.multiple).setVisibility(View.VISIBLE);

			findViewById(R.id.open).setVisibility(View.INVISIBLE);
			findViewById(R.id.comfrim).setVisibility(View.INVISIBLE);
			findViewById(R.id.start).setVisibility(View.INVISIBLE);
			
			if(mode==0)
			{
			findViewById(R.id.mback).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern1).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern2).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern3).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern4).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern5).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern6).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern7).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern8).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern9).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern10).setVisibility(View.INVISIBLE);
			findViewById(R.id.pdiy).setVisibility(View.INVISIBLE);
			}
			
			else if(mode==1) //single
			{
				findViewById(R.id.single).setVisibility(View.INVISIBLE);
				findViewById(R.id.ddouble).setVisibility(View.INVISIBLE);
				findViewById(R.id.multiple).setVisibility(View.INVISIBLE);
				
				findViewById(R.id.mback).setVisibility(View.VISIBLE);
				findViewById(R.id.pattern1).setVisibility(View.VISIBLE);
				findViewById(R.id.pattern2).setVisibility(View.VISIBLE);
				findViewById(R.id.pattern3).setVisibility(View.VISIBLE);
				findViewById(R.id.pattern4).setVisibility(View.VISIBLE);
				findViewById(R.id.pdiy).setVisibility(View.VISIBLE);

				
				findViewById(R.id.pattern5).setVisibility(View.INVISIBLE);
				findViewById(R.id.pattern6).setVisibility(View.INVISIBLE);
				findViewById(R.id.pattern7).setVisibility(View.INVISIBLE);
				
				findViewById(R.id.pattern8).setVisibility(View.INVISIBLE);
				findViewById(R.id.pattern9).setVisibility(View.INVISIBLE);
				findViewById(R.id.pattern10).setVisibility(View.INVISIBLE);
			}
			
			else if(mode==2) //double
			{
				findViewById(R.id.single).setVisibility(View.INVISIBLE);
				findViewById(R.id.ddouble).setVisibility(View.INVISIBLE);
				findViewById(R.id.multiple).setVisibility(View.INVISIBLE);
				
				findViewById(R.id.mback).setVisibility(View.VISIBLE);
				findViewById(R.id.pattern1).setVisibility(View.INVISIBLE);
				findViewById(R.id.pattern2).setVisibility(View.INVISIBLE);
				findViewById(R.id.pattern3).setVisibility(View.INVISIBLE);
				findViewById(R.id.pattern4).setVisibility(View.INVISIBLE);
				findViewById(R.id.pdiy).setVisibility(View.INVISIBLE);

				
				findViewById(R.id.pattern5).setVisibility(View.INVISIBLE);
				findViewById(R.id.pattern6).setVisibility(View.INVISIBLE);
				findViewById(R.id.pattern7).setVisibility(View.INVISIBLE);
				
				findViewById(R.id.pattern8).setVisibility(View.VISIBLE);
				findViewById(R.id.pattern9).setVisibility(View.VISIBLE);
				findViewById(R.id.pattern10).setVisibility(View.VISIBLE);
			}
			
			else if(mode==3) //multiple
			{
				findViewById(R.id.single).setVisibility(View.INVISIBLE);
				findViewById(R.id.ddouble).setVisibility(View.INVISIBLE);
				findViewById(R.id.multiple).setVisibility(View.INVISIBLE);
				
				findViewById(R.id.mback).setVisibility(View.VISIBLE);
				
				findViewById(R.id.pattern1).setVisibility(View.INVISIBLE);
				findViewById(R.id.pattern2).setVisibility(View.INVISIBLE);
				findViewById(R.id.pattern3).setVisibility(View.INVISIBLE);
				findViewById(R.id.pattern4).setVisibility(View.INVISIBLE);
				findViewById(R.id.pdiy).setVisibility(View.INVISIBLE);

				
				findViewById(R.id.pattern5).setVisibility(View.VISIBLE);
				findViewById(R.id.pattern6).setVisibility(View.VISIBLE);
				findViewById(R.id.pattern7).setVisibility(View.VISIBLE);
				
				findViewById(R.id.pattern8).setVisibility(View.INVISIBLE);
				findViewById(R.id.pattern9).setVisibility(View.INVISIBLE);
				findViewById(R.id.pattern10).setVisibility(View.INVISIBLE);
			}
		}
		else if(flag==2) // START
		{
			findViewById(R.id.open).setVisibility(View.INVISIBLE);
			findViewById(R.id.comfrim).setVisibility(View.INVISIBLE);
			findViewById(R.id.start).setVisibility(View.VISIBLE);
			findViewById(R.id.pattern2).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern1).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern3).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern4).setVisibility(View.INVISIBLE);
			
			if(mode==0)
			{
			findViewById(R.id.mback).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern1).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern2).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern3).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern4).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern5).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern6).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern7).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern8).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern9).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern10).setVisibility(View.INVISIBLE);
			findViewById(R.id.pdiy).setVisibility(View.INVISIBLE);
			}
			
			else if(mode==1) //single
			{
				findViewById(R.id.single).setVisibility(View.INVISIBLE);
				findViewById(R.id.ddouble).setVisibility(View.INVISIBLE);
				findViewById(R.id.multiple).setVisibility(View.INVISIBLE);
				
				findViewById(R.id.mback).setVisibility(View.VISIBLE);
				findViewById(R.id.pattern1).setVisibility(View.VISIBLE);
				findViewById(R.id.pattern2).setVisibility(View.VISIBLE);
				findViewById(R.id.pattern3).setVisibility(View.VISIBLE);
				findViewById(R.id.pattern4).setVisibility(View.VISIBLE);
				findViewById(R.id.pdiy).setVisibility(View.VISIBLE);

				
				findViewById(R.id.pattern5).setVisibility(View.INVISIBLE);
				findViewById(R.id.pattern6).setVisibility(View.INVISIBLE);
				findViewById(R.id.pattern7).setVisibility(View.INVISIBLE);
				
				findViewById(R.id.pattern8).setVisibility(View.INVISIBLE);
				findViewById(R.id.pattern9).setVisibility(View.INVISIBLE);
				findViewById(R.id.pattern10).setVisibility(View.INVISIBLE);
			}
			
			else if(mode==2) //double
			{
				findViewById(R.id.single).setVisibility(View.INVISIBLE);
				findViewById(R.id.ddouble).setVisibility(View.INVISIBLE);
				findViewById(R.id.multiple).setVisibility(View.INVISIBLE);
				
				findViewById(R.id.mback).setVisibility(View.VISIBLE);
				findViewById(R.id.pattern1).setVisibility(View.INVISIBLE);
				findViewById(R.id.pattern2).setVisibility(View.INVISIBLE);
				findViewById(R.id.pattern3).setVisibility(View.INVISIBLE);
				findViewById(R.id.pattern4).setVisibility(View.INVISIBLE);
				findViewById(R.id.pdiy).setVisibility(View.INVISIBLE);

				
				findViewById(R.id.pattern5).setVisibility(View.INVISIBLE);
				findViewById(R.id.pattern6).setVisibility(View.INVISIBLE);
				findViewById(R.id.pattern7).setVisibility(View.INVISIBLE);
				
				findViewById(R.id.pattern8).setVisibility(View.VISIBLE);
				findViewById(R.id.pattern9).setVisibility(View.VISIBLE);
				findViewById(R.id.pattern10).setVisibility(View.VISIBLE);
			}
			
			else if(mode==3) //multiple
			{
				findViewById(R.id.single).setVisibility(View.INVISIBLE);
				findViewById(R.id.ddouble).setVisibility(View.INVISIBLE);
				findViewById(R.id.multiple).setVisibility(View.INVISIBLE);
				
				findViewById(R.id.mback).setVisibility(View.VISIBLE);
				
				findViewById(R.id.pattern1).setVisibility(View.INVISIBLE);
				findViewById(R.id.pattern2).setVisibility(View.INVISIBLE);
				findViewById(R.id.pattern3).setVisibility(View.INVISIBLE);
				findViewById(R.id.pattern4).setVisibility(View.INVISIBLE);
				findViewById(R.id.pdiy).setVisibility(View.INVISIBLE);

				
				findViewById(R.id.pattern5).setVisibility(View.VISIBLE);
				findViewById(R.id.pattern6).setVisibility(View.VISIBLE);
				findViewById(R.id.pattern7).setVisibility(View.VISIBLE);
				
				findViewById(R.id.pattern8).setVisibility(View.INVISIBLE);
				findViewById(R.id.pattern9).setVisibility(View.INVISIBLE);
				findViewById(R.id.pattern10).setVisibility(View.INVISIBLE);
			}
		}
		else if(flag==3) //confrim
		{
			findViewById(R.id.open).setVisibility(View.INVISIBLE);
			findViewById(R.id.comfrim).setVisibility(View.VISIBLE);
			findViewById(R.id.start).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern2).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern1).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern3).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern4).setVisibility(View.INVISIBLE);
			findViewById(R.id.mback).setVisibility(View.INVISIBLE);

			findViewById(R.id.pdiy).setVisibility(View.INVISIBLE);

			
			findViewById(R.id.pattern5).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern6).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern7).setVisibility(View.INVISIBLE);
			
			findViewById(R.id.pattern8).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern9).setVisibility(View.INVISIBLE);
			findViewById(R.id.pattern10).setVisibility(View.INVISIBLE);
		}
	
	}
	
	
	private void sendview()
	{
		viewcount=2;
		findViewById(R.id.Back).setOnClickListener(listener);
		
		findViewById(R.id.Up).setOnClickListener(listener);
		findViewById(R.id.Down).setOnClickListener(listener);
		findViewById(R.id.Left).setOnClickListener(listener);
		findViewById(R.id.Right).setOnClickListener(listener);
		
		findViewById(R.id.cUp).setOnClickListener(listener);
		findViewById(R.id.cDown).setOnClickListener(listener);
		findViewById(R.id.cLeft).setOnClickListener(listener);
		findViewById(R.id.cRight).setOnClickListener(listener);
		
		findViewById(R.id.liftUp).setOnClickListener(listener);
		findViewById(R.id.liftDown).setOnClickListener(listener);
	}
	
	
	
	
	
	
	public static Bitmap getHttpBitmap(String url){
    	URL myFileURL;
    	Bitmap bitmap=null;
    	try{
    		myFileURL = new URL(url);
    		//获得连接
    		HttpURLConnection conn=(HttpURLConnection)myFileURL.openConnection();
    		//设置超时时间为6000毫秒，conn.setConnectionTiem(0);表示没有时间限制
    		conn.setConnectTimeout(6000);
    		//连接设置获得数据流
    		conn.setDoInput(true);
    		//不使用缓存
    		conn.setUseCaches(false);
    		//这句可有可无，没有影响
    		//conn.connect();
    		//得到数据流
    		InputStream is = conn.getInputStream();
    		//解析得到图片
    		bitmap = BitmapFactory.decodeStream(is);
    		//关闭数据流
    		is.close();
    	}catch(Exception e){
    		e.printStackTrace();
    	}
    	
		return bitmap;
    	
    }
	
	
	
	
	
	private ISocketResponse socketListener=new ISocketResponse() {
		
		@Override
		public void onSocketResponse(final String txt) {
		runOnUiThread(new Runnable() {
				public void run() {
				
					Log.v("equals",txt);
					if(txt.equals("cr"))
					{
						status.setText("connected");
						flag=1;
						connected=1;
						initView();
						
						
						if(degree<10)
						{
							packet.pack("00"+degree);
							user.send(packet);
						}
						else if(degree<100)
						{
							packet.pack("0"+degree);
							user.send(packet);
						}
						else
						{
							packet.pack(""+degree);
							user.send(packet);
						}
					}
					
					else if(txt.equals("sm"))
					{
						status.setText("start");
						flag=3;
						initView();
					}
					
					else if(txt.equals("cp"))
					{
						status.setText("confrim");
						Log.v("player","play");
						player.play(soundMap.get(1), 1, 1, 10,0, 1);
						flag=1;
						initView();
					}
					
					else if(txt.equals("p1"))
					{
						status.setText("pattern 1");
						flag=2;
						initView();
						 imageView.setImageDrawable(getResources().getDrawable(R.drawable.pattern1));

					}
					
					else if(txt.equals("p2"))
					{
						status.setText("pattern 2");
						flag=2;
						initView();
						 imageView.setImageDrawable(getResources().getDrawable(R.drawable.pattern2));

					}
					
					else if(txt.equals("p3"))
					{
						status.setText("pattern 3");
						flag=2;
						initView();
						 imageView.setImageDrawable(getResources().getDrawable(R.drawable.pattern3));

					}
					
					else if(txt.equals("p4"))
					{
						status.setText("pattern 4");
						flag=2;
						initView();
						 imageView.setImageDrawable(getResources().getDrawable(R.drawable.pattern4));

					}
					
					else if(txt.equals("pdiy"))
					{
						status.setText("pdiy");
						flag=2;
						initView();
						 imageView.setImageDrawable(getResources().getDrawable(R.drawable.pattern4));

					}
					
					else if(txt.equals("p5"))
					{
						status.setText("pattern 5");
						flag=2;
						initView();
						 imageView.setImageDrawable(getResources().getDrawable(R.drawable.pattern4));

					}
					
					
					else if(txt.equals("p6"))
					{
						status.setText("pattern 6");
						flag=2;
						initView();
						 imageView.setImageDrawable(getResources().getDrawable(R.drawable.pattern4));

					}
					
					else if(txt.equals("p7"))
					{
						status.setText("pattern 7");
						flag=2;
						initView();
						 imageView.setImageDrawable(getResources().getDrawable(R.drawable.pattern4));

					}
					
					else if(txt.equals("p8"))
					{
						status.setText("pattern 8");
						flag=2;
						initView();
						 imageView.setImageDrawable(getResources().getDrawable(R.drawable.pattern4));

					}
					
					else if(txt.equals("p9"))
					{
						status.setText("pattern 9");
						flag=2;
						initView();
						 imageView.setImageDrawable(getResources().getDrawable(R.drawable.pattern4));

					}
					
					else if(txt.equals("pa"))
					{
						status.setText("pattern 10");
						flag=2;
						initView();
						 imageView.setImageDrawable(getResources().getDrawable(R.drawable.pattern4));

					}
					
					/*
					else if(txt.equals("Up"))
					{
						status.setText("car forward");
					}
					
					else if(txt.equals("Do"))
					{
						status.setText("car backward");
					}
					
					else if(txt.equals("Le"))
					{
						status.setText("car left");
					}
					
					else if(txt.equals("Ri"))
					{
						status.setText("car right");
					}
					
					
					else if(txt.equals("cU"))
					{
						status.setText("camera forward");
					}
					
					else if(txt.equals("cD"))
					{
						status.setText("camera backwrd");
					}
					
					else if(txt.equals("cL"))
					{
						status.setText("camera left");
					}
					
					else if(txt.equals("cR"))
					{
						status.setText("camera right");
					}
					
					else if(txt.equals("lu"))
					{
						status.setText("lift left");
					}
					
					else if(txt.equals("ld"))
					{
						status.setText("lift right");
					}
					*/
					
					
				else if(txt.equals("fa"))
					{
					player.play(soundMap.get(1), 1, 1, 10,0, 1);
					if(viewcount==1){
					status.setText("finished_ack");
					
					
					
					try {
						Thread.currentThread();
						Thread.sleep(2000);
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					
					
					
					
				    
				    SimpleDateFormat dateformat=new SimpleDateFormat("yyyy-MM-dd_HH-mm");
			
				    Log.v("url",dateformat.format(new Date()).toString());
				  String urlStr  =  "http://"+camera_ip.toString()+":8080/v/photo/photo_"+dateformat.format(new Date()).toString()+".jpg";
//				  System.out.println(urlStr); 
				  //	"http://192.168.43.1:8080/photoaf.jpg";
				  
				    
//				    String urlStr  ="http://"+camera_ip.toString()+":8080/photoaf.jpg";
				    Log.v("url",urlStr);
				 Bitmap bitmap = getHttpBitmap(urlStr);
				 
				 if(bitmap==null)
				 {
					 urlStr  ="http://"+camera_ip.toString()+":8080/photoaf.jpg";
					 bitmap = getHttpBitmap(urlStr);
				 }
				 
					 imageView.setImageBitmap(bitmap);
					 
					 
					 
//					 saveBitmap(bitmap);
					 
				}
				
					
				}
				}
			});
		}
	};
	
	private OnClickListener listener=new OnClickListener() {
		
		@SuppressWarnings("deprecation")
		@Override
		public void onClick(View v) {
			switch(v.getId())
			{
			
				case R.id.open:
					if(connected==0)
					{
					user.open(board_ip, 60000);
					}
					else
					{
						connected=0;
						user.close();
						user.open(board_ip, 60000);
					}
					packet.pack("cr");
					user.send(packet);
					
				
					
					
					break;
		
				case R.id.start:	
					packet.pack("sm");
					user.send(packet);
					break;
					

				case R.id.reset:	
					   recreate(); 
					break;
					
				case R.id.comfrim:	
					
			//		play(int soundID, float leftVolume, float rightVolume, int priority, int loop, float rate) ,其中leftVolume和rightVolume表示左右音量，priority表示优先级,loop表示循环次数,rate表示速率，
//					player.play(soundMap.get(1), 1, 1, 10,0, 1);
					packet.pack("cp");
					user.send(packet);
					
					break;
					
				case R.id.next:	
					setContentView(R.layout.second);
					sendview();
					  myWebView = (WebView) findViewById(R.id.WebView);
					
					WebSettings setting=myWebView.getSettings();
					
	                  setting.setJavaScriptEnabled(true);     
	                  setting.setPluginState(PluginState.ON);
	                  setting.setAllowFileAccess(true);
	                  setting.setSupportZoom(true); //可以缩放
	                  setting.setBuiltInZoomControls(true); //显示放大缩小 controler                //设置出现缩放工具
	                  
	                  setting.setLayoutAlgorithm(LayoutAlgorithm.SINGLE_COLUMN);

	                  myWebView.setWebViewClient(new WebViewClient(){  
	                      @Override  
	                      public boolean shouldOverrideUrlLoading(WebView view, String url) {  
	                          view.loadUrl(url);  
	                          return true;  
	                      }  
	                  });  
	                  
	                  
	                  myWebView.addJavascriptInterface(new Object() {         
	                      public void clickOnAndroid() {         
	                          View mHandler = null;
							mHandler.post(new Runnable() {         
	                              public void run() {         
	                            	  myWebView.loadUrl("javascript:wave()");         
	                              }         
	                          });         
	                      }         
	                  }, "demo"); 
//					
	                  
//	                
//	                	  WebView mWebFlash = (WebView) findViewById(R.id.WebView);
//	                	 WebSettings settings = mWebFlash.getSettings();
//	                	 settings.setPluginState(PluginState.ON);
//	                	 settings.setJavaScriptEnabled(true);
//	                	 settings.setAllowFileAccess(true);
//	                	 settings.setDefaultTextEncodingName("GBK");
//	                	  mWebFlash.setBackgroundColor(0);
	                	String urlweb="http://"+camera_ip.toString()+":8080/jsfs.html";
//	                	  mWebFlash.loadUrl(urlweb);
//	                	 
//	                  
	                  
				
					myWebView.loadUrl(urlweb);
					break;
					
					
				case R.id.Back:	
					
					setContentView(R.layout.activity_main);
					initView();
					break;
					
					
				case R.id.Up:
					
					packet.pack("Up");
					
					user.send(packet);

					break;
					
					
					
				case R.id.Left:
					
					
					packet.pack("Le");
					
					user.send(packet);
					break;
					
				case R.id.Right:
				
				packet.pack("Ri");
					
					user.send(packet);
					break;
					
				case R.id.Down:
				packet.pack("Do");
					
					user.send(packet);
					break;
					
				case R.id.cUp:
					packet.pack("cU");
					
					user.send(packet);
					break;
					
					
					
				case R.id.cLeft:
					
						packet.pack("cL");
				user.send(packet);
					break;
					
				case R.id.cRight:
					packet.pack("cR");
				
					user.send(packet);
					break;
					
				case R.id.cDown:
					
				
				packet.pack("cD");
					
					user.send(packet);
					break;
					
					
				case R.id.liftUp:	
				
				packet.pack("lu");
					
					user.send(packet);
					break;
					
					
				case R.id.liftDown:	
					packet.pack("ld");
					
					user.send(packet);
			
					break;
					
					
				case R.id.pattern1:	
					
			packet.pack("p1");
					
					user.send(packet);
					break;
					
					
				case R.id.pattern2:	
					
				packet.pack("p2");
					user.send(packet);
					break;

					
				case R.id.pattern3:	
					
					packet.pack("p3");
							
							user.send(packet);
							break;
							
							
						case R.id.pattern4:	
							
						packet.pack("p4");
							user.send(packet);
							break;
							
							
							
						case R.id.pattern5:	
							
						packet.pack("p5");
							user.send(packet);
							break;
							
						case R.id.pattern6:	
							
							packet.pack("p6");
								user.send(packet);
								break;
								
						case R.id.pdiy:	
							
							packet.pack("pdiy-000-000-000-000");
								user.send(packet);
								break;
								
								
						case R.id.pattern7:	
							
							packet.pack("p7");
								user.send(packet);
								break;
								
						case R.id.pattern8:	
							
							packet.pack("p8");
								user.send(packet);
								break;
								
						case R.id.pattern9:	
							
							packet.pack("p9");
								user.send(packet);
								break;
								
						case R.id.pattern10:	
							
							packet.pack("p10");
								user.send(packet);
								break;
								
						case R.id.mback:	
							mode=0;
							flag=1;
							initView();
							
								break;
								
						case R.id.single:	
							mode=1;
							initView();
							
								break;
					
						case R.id.ddouble:	
							mode=2;
							initView();
							
								break;
								
						case R.id.multiple:	
							mode=3;
							initView();
							
								break;
							
			}
		}
	};
	
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		
		 	if(keyCode==KeyEvent.KEYCODE_BACK)
		{
			android.os.Process.killProcess(android.os.Process.myPid());
		}
		      
		
		return super.onKeyDown(keyCode, event);
	}
	
	
	
	public void saveBitmap( Bitmap bitmap) {
		String picName="pic.JPG";
		  File f = new File("/sdcard/DCIM/intelcup/", picName);
		  if (f.exists()) {
		   f.delete();
		  }
		  try {
		   FileOutputStream out = new FileOutputStream(f);
		   bitmap.compress(Bitmap.CompressFormat.PNG, 90, out);
		   out.flush();
		   out.close();
	
		  } catch (FileNotFoundException e) {
		   // TODO Auto-generated catch block
		   e.printStackTrace();
		  } catch (IOException e) {
		   // TODO Auto-generated catch block
		   e.printStackTrace();
		  }

		 }

		 
	
	
	
}
