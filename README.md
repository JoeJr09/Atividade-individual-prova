# Sistema de Gerenciamento de Dados de Sensores

Este projeto implementa um sistema para gerenciar e consultar dados de sensores industriais, composto por três programas principais.

## Programas

### 1. Organizador (organizador.c)

Organiza os dados dos sensores em arquivos separados, ordenados por timestamp em ordem decrescente.

**Compilação:**
```bash
gcc -o organizador organizador.c
```

**Uso:**
```bash
./organizador arquivo_entrada.txt
```

### 2. Consulta (consulta.c)

Realiza buscas binárias em arquivos de sensores para encontrar a leitura mais próxima de um timestamp específico.

**Compilação:**
```bash
gcc -o consulta consulta.c
```

**Uso:**
```bash
./consulta sensor_id timestamp
```

### 3. Gerador (gerador.c)

Gera arquivos de teste com dados aleatórios para diferentes tipos de sensores.

**Compilação:**
```bash
gcc -o gerador gerador.c
```

**Uso:**
```bash
./gerador inicio_dia inicio_mes inicio_ano inicio_hora inicio_min inicio_seg fim_dia fim_mes fim_ano fim_hora fim_min fim_seg sensor1 tipo1 [sensor2 tipo2 ...]
```

**Tipos de dados suportados:**
- CONJ_Z: Números inteiros
- CONJ_Q: Números racionais (float)
- TEXTO: Strings (até 16 caracteres)
- BINARIO: Valores booleanos (true/false)

## Exemplo de Uso

1. Gerar dados de teste:
```bash
./gerador 1 1 2024 0 0 0 31 12 2024 23 59 59 sensor1 CONJ_Z sensor2 CONJ_Q sensor3 TEXTO sensor4 BINARIO
```

2. Organizar os dados:
```bash
./organizador dados_teste.txt
```

3. Consultar um sensor:
```bash
./consulta sensor1 1704067200
```

## Requisitos

- Compilador GCC
- Sistema operacional Unix/Linux ou Windows com ambiente de desenvolvimento C 