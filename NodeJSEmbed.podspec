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

	s.source_files = 'src/NodeJSEmbed/**/*'
  
	# s.resource_bundles = {
	#   'AsyncCpp' => ['AsyncCpp/Assets/*.png']
	# }

	s.public_header_files = 'src/NodeJSEmbed/**/*.hpp'
	s.header_mappings_dir = 'src/NodeJSEmbed'
	s.pod_target_xcconfig = {
		'HEADER_SEARCH_PATHS' => '"$(PODS_ROOT)/NodeJSEmbed/src"',
		'CLANG_CXX_LANGUAGE_STANDARD' => 'gnu++17'
	}
	# s.frameworks = 'UIKit', 'MapKit'
	s.ios.dependency 'NodeMobile' #, :git => 'https://github.com/JaneaSystems/nodejs-mobile.git'
	
	s.ios.script_phase = { :name => 'Build NodeJS Headers', :script => './external/nodejs/build_headers.sh' }
	s.osx.script_phase = { :name => 'Build NodeJS', :script => './external/nodejs/build.sh' }
end