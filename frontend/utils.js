// Returns if a value is a string
function isString (value) {
    return typeof value === 'string' || value instanceof String;
}
function isArray (value) {
    return value && typeof value === 'object' && value.constructor === Array;
}