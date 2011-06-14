/*package com.view;


import android.app.Activity;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.LinearGradient;
import android.graphics.Paint;
import android.graphics.Shader;
import android.os.Bundle;
import android.view.View;

public class view extends Activity {
    
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(new ViewWithRedDot(this));
        
    }
    
    
    
    private static class ViewWithRedDot extends View {
    	public ViewWithRedDot(Context context) {
    		super(context);
    	}
    	
    	@Override
    	protected void onDraw(Canvas canvas) {
    		canvas.drawColor(Color.BLACK);
    		Paint circlePaint = new Paint();
    		circlePaint.setColor(Color.RED);
    		canvas.drawCircle(canvas.getWidth()/2,
    		canvas.getHeight()/2,
    		canvas.getWidth()/3, circlePaint);
    		Paint circlePaint = new Paint(Paint.ANTI_ALIAS_FLAG);
    		LinearGradient linGrad = new LinearGradient(0, 0, 25, 25,
    		Color.RED, Color.BLACK,
    		Shader.TileMode.MIRROR);
    		circlePaint.setShader(linGrad);
    		canvas.drawCircle(100, 100, 100, circlePaint);
    	}
    	}

}

*/
package com.view;


import java.io.ByteArrayOutputStream;
import java.io.FileNotFoundException;
import java.util.ArrayList;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;

import com.view.Base64;
import com.view.R;
import com.view.Base64.InputStream;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;



public class view extends Activity {
	Bitmap bitmap;
	TextView textTargetUri;
	TextView flag;
	ImageView targetImage;
	InputStream is;

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		Button buttonLoadImage = (Button)findViewById(R.id.loadimage);
		Button buttonSendImage = (Button)findViewById(R.id.sendimage);
		textTargetUri = (TextView)findViewById(R.id.targeturi);
		flag = (TextView)findViewById(R.id.flag);
		targetImage = (ImageView)findViewById(R.id.targetimage);
		flag.setText("start");
		buttonLoadImage.setOnClickListener(new Button.OnClickListener(){
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
 	 			Intent intent = new Intent(Intent.ACTION_PICK,
 	 					android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
 	 			startActivityForResult(intent, 0);
			}});
		
		
		buttonSendImage.setOnClickListener(new Button.OnClickListener(){
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				flag.setText("try");
				ByteArrayOutputStream bao = new ByteArrayOutputStream();
				bitmap.compress(Bitmap.CompressFormat.JPEG, 90, bao);
				byte [] ba = bao.toByteArray();
				String ba1=Base64.encodeBytes(ba);
				ArrayList<NameValuePair> nameValuePairs = new
				ArrayList<NameValuePair>();
				nameValuePairs.add(new BasicNameValuePair("image",ba1));
				flag.setText("try1");
				try{
					
					HttpClient httpclient = new DefaultHttpClient();
					HttpPost httppost = new
					HttpPost("http://bxiong.com/test.php");
					httppost.setEntity(new UrlEncodedFormEntity(nameValuePairs));
					HttpResponse response = httpclient.execute(httppost);
					HttpEntity entity = response.getEntity();
					is = (InputStream) entity.getContent();
					
					
					//add flag to indicate it works
				}catch(Exception e){
					Log.e("log_tag", "Error in http connection "+e.toString());}
			}});
         
    buttonTakePicture.setOnClickListener(new Button.OnClickListener(){
    //Button to activate the camera
			@Override
			public void onClick(View v) {
				Intent takePicture = new Intent(view.this, AndroidCamera.class);
				startActivity(takePicture);
			}
			
		});
}
    

 	@Override
 	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
 		// TODO Auto-generated method stub
 		
 		super.onActivityResult(requestCode, resultCode, data);

 		if (resultCode == RESULT_OK){
 			Uri targetUri = data.getData();
 			textTargetUri.setText(targetUri.toString());
 			
 			try {
 				bitmap = BitmapFactory.decodeStream(getContentResolver().openInputStream(targetUri));
 				targetImage.setImageBitmap(bitmap);
 				
 			} catch (FileNotFoundException e) {
 				// TODO Auto-generated catch block
 				e.printStackTrace();
 			}
 		}		
 	}
}
/*	ByteArrayOutputStream bao = new ByteArrayOutputStream();
	bitmap.compress(Bitmap.CompressFormat.JPEG, 90, bao);
	byte [] ba = bao.toByteArray();
	String ba1=Base64.encodeBytes(ba);
	ArrayList<NameValuePair> nameValuePairs = new
	ArrayList<NameValuePair>();
	nameValuePairs.add(new BasicNameValuePair("image",ba1));
	try{
		HttpClient httpclient = new DefaultHttpClient();
		HttpPost httppost = new
		HttpPost("http://bxiong.com/test.php");
		httppost.setEntity(new UrlEncodedFormEntity(nameValuePairs));
		HttpResponse response = httpclient.execute(httppost);
		HttpEntity entity = response.getEntity();
		is = (InputStream) entity.getContent();
		
		//add flag to indicate it works
	}catch(Exception e){
		Log.e("log_tag", "Error in http connection "+e.toString());
		
		
	}*/
	
	