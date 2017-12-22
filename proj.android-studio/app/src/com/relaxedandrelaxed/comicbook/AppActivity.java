/****************************************************************************
Copyright (c) 2015 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package com.relaxedandrelaxed.comicbook;

import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.view.View;
import android.view.ViewGroup;
import android.os.Bundle;

import org.cocos2dx.lib.Cocos2dxActivity;
import com.google.android.gms.ads.MobileAds;
import com.google.android.gms.ads.*;

import android.widget.RelativeLayout;
import android.view.ViewGroup.LayoutParams;
import android.view.WindowManager;
import android.graphics.Point;
import android.util.Log;
import android.widget.Toast;

import android.telephony.*;



public class AppActivity extends Cocos2dxActivity {

    private static AppActivity mActivity = null;
    private AdView adView;

    private AdRequest adRequest;


    private InterstitialAd mInterstitialAd;

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mActivity = this;

        MobileAds.initialize(this, "ca-app-pub-9291877653530829~9147473385");

        adView = new AdView(this);
        adView.setAdUnitId("ca-app-pub-9291877653530829/6517440613");
        adView.setAdSize(AdSize.BANNER);

        RelativeLayout layout = new RelativeLayout(this);
        addContentView(layout, new LayoutParams(LayoutParams.WRAP_CONTENT,LayoutParams.WRAP_CONTENT));
        layout.addView(adView);

        RelativeLayout.LayoutParams view_para = new RelativeLayout.LayoutParams(AdSize.BANNER.getWidthInPixels(this), AdSize.BANNER.getHeightInPixels(this));
        view_para.addRule(RelativeLayout.CENTER_HORIZONTAL, RelativeLayout.TRUE);
        view_para.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM,RelativeLayout.TRUE);
        adView.setLayoutParams(view_para);

        adRequest = new AdRequest.Builder().build();
        adView.loadAd(adRequest);
        adView.setAdListener(new AdListener() {
            @Override
            public void onAdClosed() {
                super.onAdClosed();
            }

            @Override
            public void onAdFailedToLoad(int i) {
                super.onAdFailedToLoad(i);
            }

            @Override
            public void onAdLeftApplication() {
                super.onAdLeftApplication();
            }

            @Override
            public void onAdOpened() {
                super.onAdOpened();
            }

            @Override
            public void onAdLoaded() {
                super.onAdLoaded();
            }
        });

        mInterstitialAd = new InterstitialAd(this);
        mInterstitialAd.setAdUnitId("ca-app-pub-9291877653530829/5798846191");
        mInterstitialAd.loadAd(adRequest);
        mInterstitialAd.setAdListener(new AdListener() {
            @Override
            public void onAdClosed() {
                super.onAdClosed();
                mInterstitialAd.loadAd(adRequest);
            }

            @Override
            public void onAdFailedToLoad(int i) {
                super.onAdFailedToLoad(i);
            }

            @Override
            public void onAdLeftApplication() {
                super.onAdLeftApplication();
            }

            @Override
            public void onAdOpened() {
                super.onAdOpened();
            }

            @Override
            public void onAdLoaded() {
                super.onAdLoaded();

            }

        });

    }

    public static Object getInstance() {
        return mActivity;
    }

    @Override
    protected void onResume() {
        // TODO Auto-generated method stub
        super.onResume();

            adView.resume();
    }

    @Override
    protected void onDestroy() {
        // TODO Auto-generated method stub
        super.onDestroy();
            adView.destroy();
    }

    public void showBanner() {
//        AdRequest rdRequest = new AdRequest.Builder().build();
//        adView.loadAd(rdRequest);

    }

    public void showFullScreen() {

        Log.d("TAG", "fuck!");

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (mInterstitialAd.isLoaded())
                {
                    mInterstitialAd.show();
                }else
                {
                    mInterstitialAd.loadAd(adRequest);

                }
            }
        });
    }

    public void openGooglePlayStore()
    {
        String url = "https://play.google.com/store/apps/details?id=" + this.getPackageName();
        Uri uri = Uri.parse(url);
        Intent it = new Intent(Intent.ACTION_VIEW, uri);
        mActivity.startActivity(it);
    }

    public void sendEmailToUs()
    {
        String[] ss = new String[]{"houhuangzb@gmail.com"};

        Intent email = new Intent(android.content.Intent.ACTION_SEND);
/*不带附件发送邮件*/
        email.setType("plain/text");
/*设置邮件默认地址，多个收件人，String数组*/
        email.putExtra(android.content.Intent.EXTRA_EMAIL, ss);
/*邮件标题*/
        email.putExtra(android.content.Intent.EXTRA_SUBJECT, "你的建议");

//		NSString *emailContent = @"Please describe your problem or suggestion below\n-\n\n\n\n\n\n";
//		NSString *appName = @"App name: Colorjoy";
//		NSString *systemType = @"System name:IOS";
//		NSString *systemsss = @"System version:";
//		NSString *systemVer = [[UIDevice currentDevice] systemVersion];
//		NSString *appVer = @"App Version: 2.4.0";

/*邮件正文*/
        String ssg = new String();
        ssg += "请描述你遇到的问题或者建议：\n\n";

        email.putExtra(android.content.Intent.EXTRA_TEXT, ssg);

//调用系统的邮件系统
        startActivity(Intent.createChooser(email, " "));
    }

    public void shareToFriend()
    {
        String ss = new String();
        ss += "如果你觉得这个APP有趣的话，赶快叫上朋友一起吧！\n";
        ss += "https://play.google.com/store/apps/details?id=" + this.getPackageName();
        Intent textIntent = new Intent(Intent.ACTION_SEND);
        textIntent.setType("text/plain");
        textIntent.putExtra(Intent.EXTRA_TEXT, ss);
        startActivity(Intent.createChooser(textIntent, "漫画大全"));

    }

    public static  Uri getResourceUri(int resId)
    {
        return Uri.parse("android.resource://"+mActivity.getPackageName()+"/"+resId);
    }

}
