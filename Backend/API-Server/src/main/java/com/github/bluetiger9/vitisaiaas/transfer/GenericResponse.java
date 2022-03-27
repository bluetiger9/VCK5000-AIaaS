/*
 * Copyright © 2022 Attila Tőkés.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 */
package com.github.bluetiger9.vitisaiaas.transfer;

import lombok.AllArgsConstructor;
import lombok.Getter;

@AllArgsConstructor
@Getter
public class GenericResponse {
    private final Object data;
}
