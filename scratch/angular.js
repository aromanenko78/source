function TodoController($scope) {
	$scope.todos = [
		{text:'mow the lawn', done:true},
		{text:'go to concert', done:false}];

	$scope.remaining = function() {
		var count = 0;
		angular.forEach($scope.todos, function(todo) {
			if (!todo.done) {
				count++;
			}
		});
		return count;
	};

	$scope.addTodo = function() {
		$scope.todos.push(
			{text:$scope.todoText, done:false});
		$scope.todoText = '';
	}

	$scope.archive = function() {
		$newTodos = [];
		angular.forEach($scope.todos, function(todo) {
			if (!todo.done) {
				$newTodos.push(todo);
			}
		});
		$scope.todos = $newTodos;
	}
}