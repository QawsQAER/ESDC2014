package com.boyaa.chat.demo;
	import android.app.Activity;  

import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;  
import java.net.URLConnection;  
import java.io.IOException;
import java.io.InputStream;  

	
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;





	import android.os.Bundle;  
import android.os.Environment;  
import android.widget.MediaController;  
import android.widget.VideoView;  
import android.widget.ImageView;  
import android.app.Activity;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.EditText;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;







import android.graphics.drawable.Drawable;

import com.boyaa.chat.R;
import com.boyaa.push.lib.service.Client;
import com.boyaa.push.lib.service.ISocketResponse;
import com.boyaa.push.lib.service.Packet;

public class MainActivity extends Activity {

	private Client user=null;
	private EditText ip,port,recContent;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		
	
        //显示
    
     
		initView();
		user=new Client(this.getApplicationContext(),socketListener);
		
		
		
	}
	


	
	private void initView()
	{
		
		findViewById(R.id.open).setOnClickListener(listener);
		findViewById(R.id.comfrim).setOnClickListener(listener);
		findViewById(R.id.start).setOnClickListener(listener);
		
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
		findViewById(R.id.pattern1).setOnClickListener(listener);
		findViewById(R.id.pattern2).setOnClickListener(listener);
		
//		findViewById(R.id.clear).setOnClickListener(listener);
		
		ip=(EditText) findViewById(R.id.ip);
		port=(EditText) findViewById(R.id.port);
//		sendContent=(EditText) findViewById(R.id.sendContent);
//		recContent=(EditText) findViewById(R.id.recContent);
		
		ip.setText("192.168.43.56");
		port.setText("60000");
		
//		recContent.setText("Connection: OFF");
		
		
		
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
				
					if(txt.equals("cr"))
					{
						port.setText("connected");
					}
					
					else if(txt.equals("sm"))
					{
						port.setText("start");
					}
					
					else if(txt.equals("cp"))
					{
						port.setText("confrim");
					}
					
					else if(txt.equals("p1"))
					{
						port.setText("pattern 1");
					}
					
					else if(txt.equals("p2"))
					{
						port.setText("pattern 2");
					}
					
					else if(txt.equals("p3"))
					{
						port.setText("pattern 3");
					}
					
					else if(txt.equals("p4"))
					{
						port.setText("pattern 4");
					}
					
					
					else if(txt.equals("Up"))
					{
						port.setText("car forward");
					}
					
					else if(txt.equals("Do"))
					{
						port.setText("car backward");
					}
					
					else if(txt.equals("Le"))
					{
						port.setText("car left");
					}
					
					else if(txt.equals("Ri"))
					{
						port.setText("car right");
					}
					
					
					else if(txt.equals("cU"))
					{
						port.setText("camera forward");
					}
					
					else if(txt.equals("cD"))
					{
						port.setText("camera backward");
					}
					
					else if(txt.equals("cL"))
					{
						port.setText("camera left");
					}
					
					else if(txt.equals("cR"))
					{
						port.setText("camera right");
					}
					
					else if(txt.equals("lu"))
					{
						port.setText("lift left");
					}
					
					else if(txt.equals("ld"))
					{
						port.setText("lift right");
					}
					
					
					
				else if(txt.equals("fa"))
					{
						port.setText("finished_ack");
					ImageView imageView;
				    imageView = (ImageView)findViewById(R.id.image_view);
				    String urlStr = "http://192.168.43.1:8080/photoaf.jpg";
				 Bitmap bitmap = getHttpBitmap(urlStr);
					 imageView.setImageBitmap(bitmap);
					}
					
				}
			});
		}
	};
	
	private OnClickListener listener=new OnClickListener() {
		Packet packet=new Packet();
		@Override
		public void onClick(View v) {
			switch(v.getId())
			{
			
				case R.id.open:
//					user.open();
//					user.close();
					user.open(ip.getText().toString(), Integer.valueOf(port.getText().toString()));
					packet.pack("cr");
					user.send(packet);
					break;
		
				case R.id.start:	
					packet.pack("sm");
					user.send(packet);
					break;
					
				case R.id.comfrim:	
					packet.pack("cp");
					user.send(packet);
					break;
					
					
				case R.id.Up:
//					packet.pack(sendContent.getText().toString());
					packet.pack("Up");
					user.send(packet);
//					sendContent.setText("");
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
//					packet.pack(sendContent.getText().toString());
					packet.pack("cU");
					user.send(packet);
//					sendContent.setText("");
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
					
//				case R.id.clear:
//					recContent.setText("");
//					break;
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
	
	
	
	
	
	
}
