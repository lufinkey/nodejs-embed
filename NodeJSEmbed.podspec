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

	s.static_framework = true

	s.source_files = 'src/embed/**/*.{c,cpp,cc,m,mm,h,hpp,inl,impl}'
	s.osx.source_files = "external/nodejs/build/desktop/$(CONFIGURATION)/*.cc"
	s.ios.frameworks = 'NodeMobile'
	s.osx.vendored_libraries = "external/nodejs/build/desktop/$(CONFIGURATION)/*.a"
  
	# s.resource_bundles = {
	#   'NodeJSEmbed' => ['NodeJSEmbed/Assets/*.png']
	# }

	s.public_header_files = "src/embed/**/*.{h,hpp,inl,impl}"
	s.private_header_files = "external/nodejs/build/addon-api/**/*.{h,hpp,inl,impl}"
	s.ios.private_header_files = "external/nodejs/build/mobile/include/**/*.{h,hpp,inl,impl}", "external/nodejs/build/mobile/node/deps/**/*.{h,hpp,inl,impl}"
	s.osx.private_header_files = "external/nodejs/build/desktop/include/**/*.{h,hpp,inl,impl}", "external/nodejs/build/desktop/node/deps/**/*.{h,hpp,inl,impl}"
	s.preserve_paths = "src/embed/nodejs/js/**/*", "external/nodejs/**/*"
	s.header_mappings_dir = "src/embed"
	s.pod_target_xcconfig = {
		'HEADER_SEARCH_PATHS' => [ "$(PODS_ROOT)/NodeJSEmbed/src", "$(PODS_ROOT)/NodeJSEmbed/src/embed/nodejs/js/build", "$(PODS_ROOT)/NodeJSEmbed/external/nodejs/build/addon-api" ],
		'CLANG_CXX_LANGUAGE_STANDARD' => 'gnu++17',
		'GCC_PREPROCESSOR_DEFINITIONS' => 'NODE_WANT_INTERNALS=1'
	}
	s.ios.pod_target_xcconfig = {
		'HEADER_SEARCH_PATHS' => [ "$(PODS_ROOT)/NodeJSEmbed/external/nodejs/build/mobile/include", "$(PODS_ROOT)/NodeJSEmbed/external/nodejs/build/mobile/include/nodejs", "$(PODS_ROOT)/NodeJSEmbed/external/nodejs/build/mobile/include/v8", "$(PODS_ROOT)/NodeJSEmbed/external/nodejs/build/mobile/include/uv" ],
		'FRAMEWORK_SEARCH_PATHS' => "$(PODS_ROOT)/NodeMobile/out_ios/Release-universal"
	}
	s.osx.pod_target_xcconfig = {
		'HEADER_SEARCH_PATHS' => [ "$(PODS_ROOT)/NodeJSEmbed/external/nodejs/build/desktop/include", "$(PODS_ROOT)/NodeJSEmbed/external/nodejs/build/desktop/include/nodejs", "$(PODS_ROOT)/NodeJSEmbed/external/nodejs/build/desktop/include/v8", "$(PODS_ROOT)/NodeJSEmbed/external/nodejs/build/desktop/include/uv", "$(PODS_ROOT)/NodeJSEmbed/external/nodejs/build/desktop/node/deps/v8", "$(PODS_ROOT)/NodeJSEmbed/external/nodejs/build/desktop/$(CONFIGURATION)" ]
	}
	# s.frameworks = 'UIKit', 'MapKit'
	s.ios.dependency 'NodeMobile' #, :git => 'https://github.com/JaneaSystems/nodejs-mobile.git'

	# build NodeJS when project is prepared
	s.ios.script_phase = {
		:name => "Prepare NodeJS Mobile Headers",
		:script => "$(PODS_TARGET_SRCROOT)/external/nodejs/build_mobile_headers.sh",
		:execution_position => :before_compile }
	s.osx.script_phase = {
		:name => "Build $(CONFIGURATION) NodeJS library for desktop",
		:script => "$(PODS_TARGET_SRCROOT)/external/nodejs/build_desktop.sh $(CONFIGURATION)",
		:execution_position => :before_compile }
	s.script_phase = {
		:name => "Build JS Module",
		:script => "$(PODS_TARGET_SRCROOT)/src/embed/nodejs/js/build.sh",
		:execution_position => :before_compile }
end
