package com.boyaa.chat.demo;
	import android.annotation.SuppressLint;
import android.app.Activity;  

import java.net.HttpURLConnection;
import java.net.URL;  
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;  

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.media.AudioManager;
import android.media.SoundPool;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.webkit.WebSettings;
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
	private EditText ip;
	private TextView status;
	private WebView myWebView;
	
	private String camera_ip="192.168.43.1";
	private String board_ip="192.168.43.142";
	SoundPool player,play;
	HashMap<Integer,Integer> soundMap;
	

	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		
		
		
		
		setContentView(R.layout.activity_main);
		
		ip=(EditText) findViewById(R.id.ip);
		status=(TextView) findViewById(R.id.status);
		
		ip.setText(board_ip);
		status.setText("off");
		
		initView();
		
		
		
		 soundMap=new HashMap<Integer,Integer>();
	        player=new SoundPool(1,AudioManager.STREAM_MUSIC,10);
	        play=new SoundPool(1,AudioManager.STREAM_MUSIC,10);
	        play.load(this, R.raw.test, 10);
	        soundMap.put(1,player.load(this, R.raw.test, 10));
	
        //显示
		
		user=new Client(this.getApplicationContext(),socketListener);
		
		
		
	}
	


	
	private void initView()
	{
		ip=(EditText) findViewById(R.id.ip);
		status=(TextView) findViewById(R.id.status);
		
		ip.setText(board_ip);
		
		findViewById(R.id.open).setOnClickListener(listener);
		findViewById(R.id.comfrim).setOnClickListener(listener);
		findViewById(R.id.start).setOnClickListener(listener);
		
		findViewById(R.id.next).setOnClickListener(listener);
		findViewById(R.id.pattern1).setOnClickListener(listener);
		findViewById(R.id.pattern2).setOnClickListener(listener);
	
	}
	
	
	private void sendview()
	{
		
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
					}
					
					else if(txt.equals("sm"))
					{
						status.setText("start");
					}
					
					else if(txt.equals("cp"))
					{
						status.setText("confrim");
						Log.v("player","play");
						player.play(soundMap.get(1), 1, 1, 10,0, 1);
					}
					
					else if(txt.equals("p1"))
					{
						status.setText("pattern 1");
					}
					
					else if(txt.equals("p2"))
					{
						status.setText("pattern 2");
					}
					
					else if(txt.equals("p3"))
					{
						status.setText("pattern 3");
					}
					
					else if(txt.equals("p4"))
					{
						status.setText("pattern 4");
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
					status.setText("finished_ack");
					
					
					
					try {
						Thread.currentThread();
						Thread.sleep(2000);
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					
					player.play(soundMap.get(1), 1, 1, 10,0, 1);
					
					ImageView imageView;
				    imageView = (ImageView)findViewById(R.id.image_view);
				    
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
			});
		}
	};
	
	private OnClickListener listener=new OnClickListener() {
		Packet packet=new Packet();
		@SuppressWarnings("deprecation")
		@Override
		public void onClick(View v) {
			switch(v.getId())
			{
			
				case R.id.open:
					user.open(ip.getText().toString(), 60000);
					packet.pack("cr");
					user.send(packet);
					break;
		
				case R.id.start:	
					
					
					packet.pack("sm");
					user.send(packet);
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
					
	                  
	                 
	                  
	                  
					String urlweb="http://"+camera_ip.toString()+":8080/jsfs.html";
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
