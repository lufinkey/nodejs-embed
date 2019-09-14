#
# Be sure to run `pod lib lint NodeJSEmbed.podspec' to ensure this is a
# valid spec before submitting.
#
# Any lines starting with a # are optional, but their use is encouraged
# To learn more about a Podspec see https://guides.cocoapods.org/syntax/podspec.html
#

Pod::Spec.new do |s|
	s.name             = 'NodeJSEmbed'
	s.version          = '0.1.0'
	s.summary          = 'An easily embeddable version of NodeJS'

# This description is used to generate tags and improve search results.
#   * Think: What does it do? Why did you write it? What is the focus?
#   * Try to keep it short, snappy and to the point.
#   * Write the description between the DESC delimiters below.
#   * Finally, don't worry about the indent, CocoaPods strips it!

	s.description      = <<-DESC
	An easily embeddable version of NodeJS
							DESC

	s.homepage = 'https://github.com/lufinkey/nodejs-embed'
	# s.screenshots     = 'www.example.com/screenshots_1', 'www.example.com/screenshots_2'
	# s.license          = { :type => 'MIT', :file => 'LICENSE' }
	s.author = { 'Luis Finke' => 'luisfinke@gmail.com' }
	s.source = { :git => 'https://github.com/lufinkey/nodejs-embed.git', :tag => s.version.to_s }
	s.social_media_url = 'https://twitter.com/lufinkey'

	s.ios.deployment_target = '9.0'
	s.osx.deployment_target = '10.14'

	s.source_files = 'src/embed/**/*.{c,cpp,cc,m,mm,h,hpp,inl,impl}'
	s.osx.source_files = 'external/nodejs/build/desktop/Release/*.cc'
	s.osx.vendored_libraries = 'external/nodejs/build/desktop/Release/*.a'
  
	# s.resource_bundles = {
	#   'NodeJSEmbed' => ['NodeJSEmbed/Assets/*.png']
	# }

	s.public_header_files = 'src/embed/**/*.{h,hpp,inl,impl}'
	s.private_header_files = 'external/nodejs/build/addon-api/**/*.{h,hpp,inl,impl}'
	s.ios.private_header_files = 'external/nodejs/build/mobile/include/**/*.{h,hpp,inl,impl}'
	s.osx.private_header_files = 'external/nodejs/build/desktop/include/**/*.{h,hpp,inl,impl}'
	s.preserve_paths = 'external/nodejs/build/addon-api/**/*.{h,hpp,inl,impl}'
	s.ios.preserve_paths = 'external/nodejs/build/mobile/include/**/*.{h,hpp,inl,impl}'
	s.osx.preserve_paths = 'external/nodejs/build/desktop/include/**/*.{h,hpp,inl,impl}'
	s.header_mappings_dir = 'src/embed'
	s.pod_target_xcconfig = {
		'HEADER_SEARCH_PATHS' => '"$(PODS_ROOT)/NodeJSEmbed/src" "$(PODS_ROOT)/NodeJSEmbed/src/embed/nodejs/js/build" "$(PODS_ROOT)/NodeJSEmbed/external/nodejs/build/addon-api"',
		'CLANG_CXX_LANGUAGE_STANDARD' => 'gnu++17',
		'GCC_PREPROCESSOR_DEFINITIONS' => 'NODE_WANT_INTERNALS=1',
		'ONLY_ACTIVE_ARCH' => 'NO'
	}
	s.ios.pod_target_xcconfig = {
		'HEADER_SEARCH_PATHS' => '"$(PODS_ROOT)/NodeJSEmbed/external/nodejs/build/mobile/include" "$(PODS_ROOT)/NodeJSEmbed/external/nodejs/build/mobile/include/nodejs" "$(PODS_ROOT)/NodeJSEmbed/external/nodejs/build/mobile/include/chakrashim" "$(PODS_ROOT)/NodeJSEmbed/external/nodejs/build/mobile/include/chakracore" "$(PODS_ROOT)/NodeJSEmbed/external/nodejs/build/mobile/include/uv"'
	}
	s.osx.pod_target_xcconfig = {
		'HEADER_SEARCH_PATHS' => '"$(PODS_ROOT)/NodeJSEmbed/external/nodejs/build/desktop/include" "$(PODS_ROOT)/NodeJSEmbed/external/nodejs/build/desktop/include/nodejs" "$(PODS_ROOT)/NodeJSEmbed/external/nodejs/build/desktop/include/v8" "$(PODS_ROOT)/NodeJSEmbed/external/nodejs/build/desktop/include/uv"'
	}
	# s.frameworks = 'UIKit', 'MapKit'
	s.ios.dependency 'NodeMobile' #, :git => 'https://github.com/JaneaSystems/nodejs-mobile.git'

	# build NodeJS when project is prepared
	s.prepare_command = './external/nodejs/build_desktop.sh Release && ./external/nodejs/build_mobile_headers.sh && ./src/embed/nodejs/js/build.sh'
end