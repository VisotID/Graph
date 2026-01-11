#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
#include <limits>
#include <stdexcept>
#include <iomanip>
#include <fstream>
#include <sstream>

using namespace std;

/// Реализация ориентированного графа на основе матрицы смежности
/// T - тип вершин, W - тип весов рёбер
template <typename T, typename W>
class DGraph {
private:

    vector<T> vertices; // список вершин
    
    vector<vector<W>> adj; // матрица смежности, adj[i][j] = вес ребра, направленного из i в j
  
    unordered_map<T, int> indexMap; // отображение вершины в индекс
    
    W INF; // обозначение "нет ребра"

public:

    /// Конструктор по умолчанию
    /// "InVal">обозначение "нет ребра"
    DGraph(W InVal) : INF(InVal) {}

    /// Размер графа
    /// Возвращает количество вершин в графе
    int Size() const {
        return static_cast<int>(vertices.size());
    }

    /// Проверка существования вершины
    /// v - вершина графа
    /// Возвращает True - если существует, False - не существует
    bool HasVert(const T& v) const {
        return indexMap.find(v) != indexMap.end();
    }

    /// Проверка существования ребра между двумя вершинами
    /// from - 1 вершина, to - 2 вершина
    /// Возвращает True - если существует, False - не существует
    bool HasEdge(const T& from, const T& to) const {
        auto it1 = indexMap.find(from);
        auto it2 = indexMap.find(to);
        if (it1 == indexMap.end() || it2 == indexMap.end())
            return false;
        return adj[it1->second][it2->second] != INF;
    }


    /// Вес ребра
    /// from - 1 вершина, to - 2 вершина
    /// Возвращает вес ребра между двумя вершинами
    W GetEdge(const T& from, const T& to) const {
        auto it1 = indexMap.find(from);
        auto it2 = indexMap.find(to);
        if (it1 == indexMap.end() || it2 == indexMap.end())
            throw runtime_error("Вершина не найдена"); // Бросаем исключение ошибки времени выполнения
        W val = adj[it1->second][it2->second];
        if (val == INF)
            throw runtime_error("Ребро не существует"); // Бросаем исключение ошибки времени выполнения
        return val;
    }


    /// Добавление вершины
    /// Сложность: O(V ^ 2)
    /// value - значение вершины
    void AddV(const T& value) {
        if (indexMap.find(value) != indexMap.end()) // если вершина существует
        { 
            throw runtime_error("Вершина уже существует");
        }

        // Добавляем её в список вершин и карту индексов
        vertices.push_back(value);
        indexMap[value] = vertices.size() - 1;

        // Расширяем матрицу смежности: добавляем новый столбец и строку
        // Добавляем столбец INF в каждую существующую строку
        for (size_t i = 0; i < adj.size(); ++i) {
            adj[i].push_back(INF);
        }

        // Добавляем новую строку (все INF)
        adj.push_back(vector<W>(vertices.size(), INF));
    }


    /// Удаление вершины
    /// Сложность: O(V^2)
    /// value - значение вершины
    void DeleteV(const T& value) {
        if (indexMap.find(value) == indexMap.end()) // если вершина не существует
        {
            throw runtime_error("Вершина не найдена");
        }

        int idx = indexMap[value];

        vertices.erase(vertices.begin() + idx); // Удаляем вершину 

        adj.erase(adj.begin() + idx); // Удаляем соответствующую строку из матрицы смежности


        for (size_t i = 0; i < adj.size(); ++i) // Удаляем соответствующий столбец из матрицы смежности
        {
            adj[i].erase(adj[i].begin() + idx);
        }

        // Перестраиваем indexMap
        indexMap.clear();
        for (size_t i = 0; i < vertices.size(); ++i)
        {
            indexMap[vertices[i]] = i;
        }
    }

    /// Добавление или редактирование ребра 
    /// Сложность: O(1)
    /// from - 1 вершина, to - 2 вершина, weight - вес ребра
    void AddE(const T& from, const T& to, W weight) {

        if (indexMap.find(from) == indexMap.end() || indexMap.find(to) == indexMap.end())
            throw runtime_error("Одна из вершин не найдена");

        int u = indexMap[from];
        int v = indexMap[to];
        adj[u][v] = weight; // создаём или перезаписываем ребро из u в v
    }

    /// Удаление ребра 
    /// Сложность: O(1)
    /// from - 1 вершина, to - 2 вершина
    void DeleteE(const T& from, const T& to) {

        if (indexMap.find(from) == indexMap.end() || indexMap.find(to) == indexMap.end()) // если одна из вершин не существует
            throw runtime_error("Одна из вершин не найдена");

        int u = indexMap[from];
        int v = indexMap[to];
        adj[u][v] = INF; // перезаписываем ребро из u в v на значение бесконечности
    }

    /// Обход в ширину (BFS)
    /// Сложность: O(V ^ 2)
    /// start - начальная вершина
    /// Возвращаем массив с значениями вершин
    vector<T> BFS(const T& start) const {
        auto it = indexMap.find(start);
        if (it == indexMap.end()) // если начальная вершина не существует
            throw runtime_error("Начальная вершина не найдена");

        int s = it->second; // индекс стартовой вершины
        int n = Size();

        vector<char> visited(n, 0); // посещенные вершины
        vector<char> in_queue(n, 0); // вершины в очереди
        queue<int> q;
        vector<T> order;

        // Поместить стартовую вершину в очередь
        q.push(s);
        in_queue[s] = 1;

        while (!q.empty()) {

            // Извлечь вершину, отметить как посещённую
            int u = q.front();
            q.pop();

            if (visited[u])
                continue;

            visited[u] = 1;
            order.push_back(vertices[u]);

            // Проверяем всех возможных соседей
            for (int v = 0; v < n; ++v) {
                if (adj[u][v] != INF && !visited[v] && !in_queue[v]) {
                    q.push(v);
                    in_queue[v] = 1;
                }
            }
        }
        return order;
    }

    /// Обход в глубину (DFS)
    /// Сложность: O(V ^ 2)
    /// start - начальная вершина
    /// Возвращаем массив с значениями вершин
    vector<T> DFS(const T& start) const {

        auto it = indexMap.find(start);
        if (it == indexMap.end()) // Если начальная вершина не найдена
            throw runtime_error("Начальная вершина не найдена"); // Бросаем исключение ошибки времени выполнения

        int s = it->second; // индекс стартовой вершины
        int n = Size();

        vector<char> visited(n, 0); // посещенные вершины
        vector<T> order;
        stack<int> st;

        st.push(s);

        while (!st.empty()) {
            int u = st.top();
            st.pop();

            if (visited[u])
                continue;

            visited[u] = 1;
            order.push_back(vertices[u]);

            // Добавляем всех соседей вершины u в стек
            for (int v = 0; v < n; ++v) {
                if (adj[u][v] != INF && !visited[v]) {
                    st.push(v);
                }
            }
        }

        return order;
    }

    /// Алгоритм Форда-Беллмана для поиска кратчайшего пути от стартовой вершины ко всем остальным
    /// Сложность:
    /// пустой граф O(V ^ 2)
    /// разреженный граф O(V* E)
    /// плотный граф O(V ^ 3)
    /// start - начальная вершина
    /// Возвращаем список длин кратчайших путей от стартовой вершины до всех остальных по порядку
    vector<W> BellmanFord(const T& start) const {

        if (indexMap.find(start) == indexMap.end()) // Если вершина не существует
            throw runtime_error("Вершина не найдена");

        int n = vertices.size();
        int s = indexMap.at(start); // индекс стартовой вершины

        // Инициализация: все расстояния = INF, кроме стартовой (0)
        vector<W> dist(n, INF);
        dist[s] = 0;

        // Основной цикл 
        // для каждого ребра u->v обновляем dist[v] = min(dist[v], dist[u] + w)
        for (int k = 0; k < n - 1; ++k) {
            for (int u = 0; u < n; ++u) {
                for (int v = 0; v < n; ++v) {
                    if (adj[u][v] != INF && dist[u] != INF) {
                        if (dist[v] > dist[u] + adj[u][v]) { // если существует более короткий путь в вершину v
                            dist[v] = dist[u] + adj[u][v]; // перезаписываем длину кратчайшего пути в вершину v
                        }
                    }
                }
            }
        }

        //  Проверка на отрицательные циклы 
        for (int u = 0; u < n; ++u) {
            for (int v = 0; v < n; ++v) {
                // если после основного цикла остаются более короткие пути
                if (adj[u][v] != INF && dist[u] != INF && dist[v] > dist[u] + adj[u][v]) {
                    throw runtime_error("Обнаружен отрицательный цикл!");
                }
            }
        }

        return dist;
    }

    /// Печать матрицы смежности 
    void printM() const {
        cout << "\nМатрица смежности (INF = нет ребра):\n";
        cout << setw(8) << "";
        for (size_t i = 0; i < vertices.size(); ++i)
            cout << setw(8) << vertices[i];
        cout << endl;

        for (size_t i = 0; i < vertices.size(); ++i) {
            cout << setw(8) << vertices[i];
            for (size_t j = 0; j < vertices.size(); ++j) {
                if (adj[i][j] == INF) cout << setw(8) << "INF";
                else cout << setw(8) << adj[i][j];
            }
            cout << endl;
        }
        cout << endl;
    }

    /// Возвращает список соседей вершины
    /// Сложность: O(V)
    /// vertex - вершина
    /// Возвращает массив вершин, с которыми у данной вершины есть связь в любом направлении
    vector<T> GetNeighbors(const T& vertex) const {

        auto it = indexMap.find(vertex);
        if (it == indexMap.end()) // если вершины не существует
            throw runtime_error("GetNeighbors: вершина не найдена");

        int idx = it->second;
        vector<T> neighbors;

        for (size_t j = 0; j < vertices.size(); ++j) 
        {

            // исходящие рёбра
            if (adj[idx][j] != INF) 
            {
                neighbors.push_back(vertices[j]);
            }

            // входящие рёбра
            else if (adj[j][idx] != INF) 
            {
                neighbors.push_back(vertices[j]);
            }
        }
        return neighbors;
    }

    /// Получение веса ребра между двумя вершинами
    /// Сложность: O(1)
    /// from - 1 вершина, to - 2 вершина
    /// Возвращаем вес ребра между двумя вершинами
    W GetWeight(const T& from, const T& to) const {
        auto it1 = indexMap.find(from);
        auto it2 = indexMap.find(to);
        if (it1 == indexMap.end() || it2 == indexMap.end()) // если хотя бы одной из вершин не существует
            throw runtime_error("Вершина не найдена");

        int u = it1->second;
        int v = it2->second;

        if (adj[u][v] == INF) // если ребра не существует
            throw runtime_error("Ребро отсутствует");

        return adj[u][v];
    }

    /// Сохранение графа в файл формата GraphML
    /// <param name="filename">Имя файла для сохранения</param>
    /// <exception cref="runtime_error - невозможно открыть файл">
    /// </exception>
    void saveGraphML(const string& filename) const {
        ofstream file(filename);
        if (!file.is_open()) // если файл не открывается
            throw runtime_error("Не удалось открыть файл для записи");

        file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        file << "<graphml xmlns=\"http://graphml.graphdrawing.org/xmlns\"\n";
        file << "         xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n";
        file << "         xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns\n";
        file << "         http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\">\n";

        file << "  <graph id=\"G\" edgedefault=\"directed\">\n";

        // Узлы
        for (size_t i = 0; i < vertices.size(); ++i) {
            file << "    <node id=\"n" << i << "\"";
            file << " name = " << "\"" << vertices[i] << "\"";
            file << "</node>\n";
        }

        // Рёбра
        for (size_t u = 0; u < vertices.size(); ++u) {
            for (size_t v = 0; v < vertices.size(); ++v) {
                if (adj[u][v] != INF) {
                    file << "    <edge isDirected=\"true\" source=\"n" << u
                        << "\" target=\"n" << v << "\"";
                    file << " weight = " << "\"" << adj[u][v] << "\"";
                    file << "</edge>\n";
                }
            }
        }
        file << "  </graph>\n";
        file << "</graphml>\n";
        file.close();
    }

    /// Загрузка графа из файла формата GraphML
    /// <param name="filename">Имя файла для загрузки</param>
    /// <exception cref="runtime_error - невозможно открыть файл или некорректная запись узлов/рёбер (отсутствует weight у ребра или name у узла)">
    /// </exception>
    void loadGraphML(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) // если файл не открывается
            throw runtime_error("Не удалось открыть файл для чтения"); // Бросаем исключение ошибки времени выполнения

        vertices.clear();
        adj.clear();
        indexMap.clear();

        string line;
        unordered_map<string, int> xmlIdToIndex;

        while (getline(file, line)) {

            //  Узлы 
            if (line.find("<node") != string::npos) {

                // id="n0"
                size_t idPos = line.find("id=\"");
                if (idPos == string::npos)
                    continue;
                idPos += 4; // теперь указывает на первый символ значения id
                size_t idEnd = line.find("\"", idPos);
                string xmlId = line.substr(idPos, idEnd - idPos);

                // name="A"
                size_t namePos = line.find("name=\"");
                if (namePos == string::npos)
                    throw runtime_error("Ошибка: node без name");

                namePos += 6;
                size_t nameEnd = line.find("\"", namePos);
                string value = line.substr(namePos, nameEnd - namePos);

                // Добавляем вершину
                AddV(value);
                xmlIdToIndex[xmlId] = vertices.size() - 1;

                continue;
            }

            //  Рёбра 
            if (line.find("<edge") != string::npos) {

                // source="n0"
                size_t sPos = line.find("source=\"");
                if (sPos == string::npos)
                    continue;
                sPos += 8;
                size_t sEnd = line.find("\"", sPos);
                string xmlSource = line.substr(sPos, sEnd - sPos);

                // target="n1"
                size_t tPos = line.find("target=\"");
                if (tPos == string::npos)
                    continue;
                tPos += 8;
                size_t tEnd = line.find("\"", tPos);
                string xmlTarget = line.substr(tPos, tEnd - tPos);

                // weight="5.5"
                size_t wPos = line.find("weight=\"");
                if (wPos == string::npos)
                    throw runtime_error("Ошибка: edge без weight");

                wPos += 8;
                size_t wEnd = line.find("\"", wPos);
                string wStr = line.substr(wPos, wEnd - wPos);

                // Конвертация веса
                W weight{};
                stringstream ss(wStr);
                ss >> weight;

                // Индексы
                int u = xmlIdToIndex.at(xmlSource);
                int v = xmlIdToIndex.at(xmlTarget);

                AddE(vertices[u], vertices[v], weight);
            }
        }
        file.close();
    }
};

/// Тестирование операций с вершинами (AddV, DeleteV, HasVert, size)
void test_vertex_operations();

/// Тестирование операций с рёбрами (AddE, DeleteE, HasEdge, GetEdge)
void test_edge_operations();

/// Тест BFS
void test_bfs();

/// Тест DFS
void test_dfs();

/// Тест BellmanFord
void test_bellman_ford();

/// Тест GetNeighbors
void test_get_neighbors();

/// Тест GetWeight
void test_get_weight();

/// Тест saveGraphML
void test_saveGraphML();

/// Тест loadGraphML
void test_loadGraphML();