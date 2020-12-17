//============================================================================//
//                                                                            //
//                Copyright © 2015 - 2020 Subterranean Security               //
//                                                                            //
//  This source file is subject to the terms of the Mozilla Public License    //
//  version 2. You may not use this file except in compliance with the MPL    //
//  as published by the Mozilla Foundation at:                                //
//                                                                            //
//    https://mozilla.org/MPL/2.0                                             //
//                                                                            //
//=========================================================S A N D P O L I S==//

import java.nio.ByteBuffer

import org.gradle.language.cpp.tasks.CppCompile
import org.gradle.nativeplatform.tasks.LinkExecutable

plugins {
	id("sandpolis-module")
	id("sandpolis-soi")
	id("cpp-application")
}

application {
	privateHeaders {
		from(project.file("src/main/headers"))
		from(project(":module:com.sandpolis.core.foundation").file("gen/main/cpp"))
		from(project(":module:com.sandpolis.core.instance").file("gen/main/cpp"))
		from(project(":module:com.sandpolis.core.net").file("gen/main/cpp"))
	}
	source {
		from(project.file("src/main/cpp"))
		from(project(":module:com.sandpolis.core.foundation").file("gen/main/cpp"))
		from(project(":module:com.sandpolis.core.instance").file("gen/main/cpp"))
		from(project(":module:com.sandpolis.core.net").file("gen/main/cpp"))
	}
}

tasks.withType<LinkExecutable> {
	lib(file("/usr/lib/x86_64-linux-gnu/libncursesw.a"))
	lib(file("/usr/local/lib/libprotobuf-lite.a"))
	linkerArgs.set(listOf("-ldl", "-ltinfo", "-lpthread"))
}

tasks.withType<CppCompile> {
	dependsOn(":module:com.sandpolis.core.foundation:generateProto")
	dependsOn(":module:com.sandpolis.core.instance:generateProto")
	dependsOn(":module:com.sandpolis.core.net:generateProto")
	compilerArgs.set(listOf("-pthread", "-std=c++20"))
}

// Inject resources into the executable
val injectResources by tasks.creating(DefaultTask::class) {
	dependsOn(":com.sandpolis.agent.micro:assembleDebug")
	dependsOn(":com.sandpolis.agent.micro:writeSoi")

	doLast {
		var resource_number = 1
		val executable = project.file("build/exe/main/debug/com.sandpolis.agent.micro")

		project.file("src/main/resources").walk().filter { it.isFile() }.forEach {

			// Write the file followed by the file size, path, path size, and resource index
			executable.appendBytes(it.readBytes())
			executable.appendBytes(ByteBuffer.allocate(8).putLong(it.length()).array())
			executable.appendText(it.getName())
			executable.appendBytes(ByteBuffer.allocate(4).putInt(it.getName().length).array())
			executable.appendBytes(ByteBuffer.allocate(4).putInt(resource_number).array())

			resource_number++
		}
	}
}
