# NodeJS Embed

An easy to use C++ library for embedding NodeJS into your app. Officially supports iOS and Android at the moment.

## Setup - iOS

This library can be added as a dependency via cocoapods. You must add the [NodeJS Mobile](https://github.com/janeasystems/nodejs-mobile.git) dependency as well:

```
pod 'NodeMobile', :git => 'https://github.com/janeasystems/nodejs-mobile.git'
pod 'NodeJSEmbed', :git => 'https://github.com/lufinkey/nodejs-embed.git'
```

In addition, an xcode project is provided in `projects/xcode` which can be easily linked to from another xcode project. The NodeJS Mobile dependency must still be added separately, though.

## Setup - Android

A gradle module is provided in `projects/android-studio/libnodejsembed` which can be linked to from your project 
