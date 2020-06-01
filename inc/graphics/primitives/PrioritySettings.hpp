#pragma once

/* Definiuje kolejność wywoływań Updatable::update.
 * Najpierw są aktualizowane obiekty o najmniejszym priorytecie.
 * Obiekty o priorytecie ujemnym nie są aktualizowane w trakcie wykonywania
 * się animacji typu FOREGROUND. Stałe NEGATIVE i POSITIVE nie są wykorzystane
 * w kodzie, służa "nagłówkami".
 */

enum Priority {
	NEGATIVE = -100,
	PGameWrapper,
	PAIPlayer,
	PBoardRenderable,

	POSITIVE = 0,
	POverlay,
	PPostProcessor
};