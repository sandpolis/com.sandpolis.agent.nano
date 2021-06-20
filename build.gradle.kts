//============================================================================//
//                                                                            //
//                         Copyright © 2015 Sandpolis                         //
//                                                                            //
//  This source file is subject to the terms of the Mozilla Public License    //
//  version 2. You may not use this file except in compliance with the MPL    //
//  as published by the Mozilla Foundation.                                   //
//                                                                            //
//============================================================================//

import java.nio.ByteBuffer
import java.nio.ByteOrder

import org.gradle.language.cpp.tasks.CppCompile
import org.gradle.nativeplatform.tasks.LinkExecutable

plugins {
	id("sandpolis-module")
	id("sandpolis-soi")
	id("cpp-application")
}

application {
	privateHeaders {
		from(project.file("src/main/cpp"))
	}
	source {
		from(project.file("src/main/cpp"))
	}
}

tasks.withType<LinkExecutable> {
	//linkerArgs.set(listOf("-lpthread"))
}

tasks.withType<CppCompile> {
	compilerArgs.set(listOf("-std=c++17"))
}

// Inject resources into the executable
val injectResources by tasks.creating(DefaultTask::class) {
	dependsOn(":com.sandpolis.agent.nano:assembleDebug")
	dependsOn(":com.sandpolis.agent.nano:writeSoi")

	doLast {
		val executable = project.file("build/exe/main/debug/com.sandpolis.agent.nano")

		// Add resource section header (all zeros)
		executable.appendBytes(ByteBuffer.allocate(4).order(ByteOrder.LITTLE_ENDIAN).putInt(0).array())

		project.file("src/main/resources").walk().filter { it.isFile() }.forEach {

			// Write the file followed by the file size, path, and path size
			executable.appendBytes(it.readBytes())
			executable.appendBytes(ByteBuffer.allocate(8).order(ByteOrder.LITTLE_ENDIAN).putLong(it.length()).array())
			executable.appendText(it.getName())
			executable.appendBytes(ByteBuffer.allocate(4).order(ByteOrder.LITTLE_ENDIAN).putInt(it.getName().length).array())
		}
	}
}
