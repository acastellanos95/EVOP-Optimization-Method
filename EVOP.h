//
// Created by andre on 5/13/22.
//

#ifndef EVOP__EVOP_H_
#define EVOP__EVOP_H_

#include <vector>
#include <string>
#include <cmath>

/// Regresamos arreglo de esquinas del hipercubo formado por permutaciones de sumas y restas de delta trasladado según x_0
/// \param perm : permutación inicial debe ser {-1,-1,..., -1}
/// \param x_0 : centro del hipercubo
/// \param delta : distancia de las caras al centro x_0
/// \param i : indice inicial debe ser 0
/// \param len : longitud deseada
/// \param set : arreglo de puntos a llenar
void generateVertexFromPoint(std::vector<double> &perm, const std::vector<double> &x_0, const std::vector<double> &delta, size_t i, const size_t &len, std::vector<std::vector<double>> &set){
  if(i == len){
    std::vector<double> vertex(perm.size(), 0.0);
    // Crear vértice
    for(size_t index = 0; index < perm.size(); ++index){
      vertex[index] = x_0[index] + perm[index] * delta[index];
    }
    set.push_back(vertex);
    return;
  }

  perm[i] = 1.0;
  generateVertexFromPoint(perm, x_0, delta, i+1, len, set);

  perm[i] = -1.0;
  generateVertexFromPoint(perm, x_0, delta, i+1, len, set);
}

/// Crear esquinas del hipercubo
/// \param x : centro del hipercubo
/// \param delta : distancia de las caras al centro x_0
/// \return arreglo de esquinas
std::vector<std::vector<double>> getVerticesFromCenter(const std::vector<double> &x, const std::vector<double> &delta){
  const size_t N = x.size();
  std::vector<double> perm(N, -1.0);
  std::vector<std::vector<double>> vertices;
  generateVertexFromPoint(perm, x, delta, 0, N, vertices);
  return vertices;
}

/// Obtiene la norma del vector
/// \param x : vector en R^N
/// \return ||x||
double normVector(const std::vector<double> &x){
  double result = 0.0;
  for(auto &x_i: x){
    result += x_i * x_i;
  }
  return sqrt(result);
}

/// Comparamos entre 2 vectores de precisión doble usando un epsilon
/// \param x : vector 1 en R^N
/// \param y : vector 2 en R^N
/// \param eps : epsilon mínimo de comparación
/// \return true si son iguales según ||x - y|| < eps, de lo contrario false
bool compDoubleVectors(const std::vector<double> &x, const std::vector<double> &y, double eps){
  if(x.size() != y.size())
    throw std::runtime_error("no se pueden comparar vectores de diferente tamaño");

  std::vector<double> substract(x.size(), 0.0);
  for(size_t i = 0; i < x.size(); ++i){
    substract[i] = x[i] - y[i];
  }

  if(normVector(substract) < eps)
    return true;
  return false;
}

/// Divide todos los elementos de un vector con un denominador
/// \param delta : vector a dividir
/// \param denominator : denominador
void divide(std::vector<double> &delta, double denominator){
  for(auto &delta_i: delta){
    delta_i /= denominator;
  }
}

/// Función a minimizar
/// \param x : vector en R^N
/// \return f(x)
double function(const std::vector<double> &x){
  // Implement function
  return 50.0*(x[1]-(x[0]*x[0]))*(x[1]-(x[0]*x[0])) + (2.0 - x[0])*(2.0 - x[0]);
}

/// Método EVOP de minimización de una función con N variables
/// \param x_0 : centro del hipercubo
/// \param delta : vector de deltas (distancia a caras del hipercubo en un eje)
/// \param eps : epsilon
/// \return arreglo de pares (x, f(x)) de cada iteración
std::vector<std::pair<std::vector<double>, double>> EVOP(std::vector<double> &x_0, std::vector<double> &delta, double eps){
  /* Resultados en formato (x,f(x)) */
  std::vector<std::pair<std::vector<double>, double>> results;
  // Paso 1
  std::vector<double> x = x_0;
  const size_t N = x_0.size();

  // Paso 2: Terminar si ||delta|| > epsilon
  while (normVector(delta) >= eps){

    // Paso 2: Generamos las 2^N esquinas del hipercubo y los guardamos en verticesAndPointAtEnd con el punto x al final
    std::vector<std::vector<double>> verticesAndPointAtEnd = getVerticesFromCenter(x, delta);
    verticesAndPointAtEnd.push_back(x);

    // Paso 3: Calcular los valores de la función con los puntos en verticesAndPointAtEnd y quedarse con el punto que
    // genere la menor función y asignarlo a x
    double min = std::numeric_limits<double>::max();
    for(auto &point: verticesAndPointAtEnd){
      if(function(point) < min){
        x = point;
        min = function(point);
      }
    }

    // Paso 4: Reducir parámetros delta si el punto donde es mínima la función es x_0, de otra manera encontramos un
    // nuevo centro que asignamos a x_0
    if(compDoubleVectors(x,x_0,1e-8)){
      divide(delta,2.0);
    } else {
      x_0 = x;
    }

    // Guardamos resultados de la i-ésima iteración
    results.emplace_back(x_0, function(x_0));
  }

  return results;
}

#endif //EVOP__EVOP_H_
