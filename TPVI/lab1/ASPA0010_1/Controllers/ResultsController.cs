using Microsoft.AspNetCore.Mvc;
using BSTU.Results.Collection;
using System.Net;

[ApiController]
[Route("api/[controller]")]
public class ResultsController : ControllerBase
{
    private readonly IResultsService _resultsService;

    public ResultsController(IResultsService resultsService)
    {
        _resultsService = resultsService;
    }

    [HttpGet]
    public async Task<IActionResult> GetAll()
    {
        var results = await _resultsService.GetAllAsync();
        if (results.Count == 0) return NoContent(); // 204
        return Ok(results); // 200
        // TODO: Добавить авторизацию для 401
    }

    [HttpGet("{k:int}")]
    public async Task<IActionResult> GetByKey(int k)
    {
        var value = await _resultsService.GetByKeyAsync(k);
        if (value == null) return NotFound(); // 404
        return Ok(new { key = k, value }); // 200
    }

    [HttpPost]
    public async Task<IActionResult> Add([FromBody] ResultDto dto)
    {
        if (string.IsNullOrWhiteSpace(dto?.Value)) return BadRequest(); // 400
        var (key, value) = await _resultsService.AddAsync(dto.Value);
        return CreatedAtAction(nameof(GetByKey), new { k = key }, new { key, value }); // 201
    }

    [HttpPut("{k:int}")]
    public async Task<IActionResult> Update(int k, [FromBody] ResultDto dto)
    {
        if (string.IsNullOrWhiteSpace(dto?.Value)) return BadRequest(); // 400
        var updatedValue = await _resultsService.UpdateAsync(k, dto.Value);
        if (updatedValue == null) return NotFound(); // 404
        return Ok(new { key = k, value = updatedValue }); // 200
    }

    [HttpDelete("{k:int}")]
    public async Task<IActionResult> Delete(int k)
    {
        var deletedValue = await _resultsService.DeleteAsync(k);
        if (deletedValue == null) return NotFound(); // 404
        return Ok(new { key = k, value = deletedValue }); // 200
    }
}

public class ResultDto
{
    public string Value { get; set; }
}