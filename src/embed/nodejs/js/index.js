
const native_embed = process.binding('native_embed');
const Module = require('module');
const path = require('path');

// credit to https://github.com/metacall/core for this bootstrap code

function loadModuleFromMemory(name, buffer, opts={}) {
	if (typeof name !== 'string') {
		throw new Error('name must be a string, not ' + typeof name);
	}
	if (typeof buffer !== 'string') {
		throw new Error('buffer must be a string, not ' + typeof buffer);
	}
	if (typeof opts !== 'object') {
		throw new Error('options must be an object, not ' + typeof opts);
	}
	
	const paths = Module._nodeModulePaths(path.dirname(name));
	const parent = module.parent;
	const m = new Module(name, parent);
	
	m.filename = name;
	m.paths = [
	   ...(opts.prependPaths || []),
	   ...paths,
	   ...(opts.appendPaths || []),
	];
	m._compile(buffer, name);
	return m;
}

function unloadModule(name) {
	const absolute = path.resolve(__dirname, name);
	if (require.cache[absolute]) {
		delete require.cache[absolute];
	}
}

native_embed.registerFunctions({
	loadModuleFromMemory,
	unloadModule,
	require
});
